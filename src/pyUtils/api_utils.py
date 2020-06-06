import os,time,datetime
from dateutil import tz
from base64 import b64encode
import requests
import json

def make_token(username,password):
   nameAndPass=username+":"+password
#token is base 64 encoded ant then decoded to acsii as python 3 stores it as a byte string
   token=b64encode(nameAndPass.encode('utf-8')).decode("ascii")
   return token


def get_file_headers(username,password,folder,filename):
   nretry=4
   token =make_token(username,password)
   serverurl="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "head", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
      try:
        r=requests.get(serverurl, headers=headers, params=params )
        r.raise_for_status()
        break
        time.sleep(1)
      except requests.exceptions.RequestException as e:
        if count==nretry:
           print(e)
           return 1
   if r.status_code !=200:
      print(r.content)
      r.raise_for_status()
   fileurl=r.content.decode("ascii")
   for count in  range(1,nretry):
      try:
         headr=requests.head(fileurl)
         headr.raise_for_status()
         break
         time.sleep(1)
      except requests.exceptions.RequestException as e:
         if count==nretry:
            print(e)
            return None
   if headr.status_code !=200:
      return None
   return headr.headers


def utcStr_to_local_time(lmStr):
    utc=datetime.datetime.strptime(lmStr, "%a, %d %b %Y %H:%M:%S GMT")
    utc=utc.replace(tzinfo=tz.gettz('UTC'))
    local=utc.astimezone(tz.tzlocal())
    lmTime=time.mktime(local.timetuple())
    return lmTime


def current_local_time():
    dt=datetime.datetime.now()
    return time.mktime(dt.timetuple())


def local_file_is_newer(filename,headers):
   if headers is None:
       return True
   remote_lmStr=headers["Last-Modified"]
   remote_lmTime=utcStr_to_local_time(remote_lmStr)
   local_lmTime=os.path.getmtime(filename)
   return local_lmTime>remote_lmTime

def remote_file_is_expiring(headers):
   if headers is None:
       return False
   if "x-amz-expiration" not in headers:
       return False
   remote_expStr=headers["x-amz-expiration"].replace("=",":")
   remote_expStr=remote_expStr.replace("=",":")
   remote_expStr=remote_expStr.replace('expiry-date','"expiry-date"')
   remote_expStr=remote_expStr.replace('rule-id','"rule-id"')
   remote_expStr="{"+remote_expStr+"}"
   remote_expStr=json.loads(remote_expStr)
   remote_expStr=remote_expStr['expiry-date']
   remote_expTime=utcStr_to_local_time(remote_expStr)
   day_seconds=24*3600
   min_time_delta=10*day_seconds
   is_expiring=remote_expTime-current_local_time() < min_time_delta
   return is_expiring


def upload_project_file_with_put(username,password,filename,folder):
   nretry=4
   token =make_token(username,password)
   serverurl="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "put", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
      try:
         r=requests.get(serverurl, headers=headers, params=params )
         r.raise_for_status()
         break
         time.sleep(1)
      except requests.exceptions.RequestException as e:
         if count==nretry:
            print(e)
   if r.status_code !=200:
      print(r.content)
      r.raise_for_status()
   fileurl=r.content
   for count in  range(1,nretry):
      try:
        with open(filename, 'rb') as data:
          putr=requests.put(fileurl, data=data)
          putr.raise_for_status()
          break
          time.sleep(1)
      except requests.exceptions.RequestException as e:
        if count==nretry:
          print(e)
   if putr.status_code !=200:
      print(putr.content)
      putr.raise_for_status()


def upload_project_file(username,password,filename,folder, only_if_newer=False):
   nretry=4
   if only_if_newer :
       headers=get_file_headers(username,password,folder,filename)
       if not local_file_is_newer(filename,headers):
           if not remote_file_is_expiring(headers):
                return
   token =make_token(username,password)
   serverurl="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   length=int(os.path.getsize(filename))
   params={"method": "post", "filename": filename, "folder": folder, "content-length": length}
   for count in  range(1,nretry):
      try:
        r=requests.get(serverurl, headers=headers, params=params )
        r.raise_for_status()
        break
        time.sleep(1)
      except requests.exceptions.RequestException as e:
        if count==nretry:
           print(e)
   if r.status_code !=200:
      print(r.content)
      r.raise_for_status()
   content=json.loads(r.content.decode("ascii"))
   for count in  range(1,nretry):
      try:
         postr=requests.post(content["url"], data=content["fields"],files={'file': open(filename, 'rb')})
         postr.raise_for_status()
         break
         time.sleep(1)
      except requests.exceptions.RequestException as e:
        if count==nretry:
            print(e)
   if postr.status_code !=200:
      print(postr.content)
      postr.raise_for_status()


def url_filename(fileurl):
   fname_parts=fileurl.split('?')[0].split('/')
   l=len(fname_parts)
   fname=fname_parts[l-1]
   return fname


def download_signed_url(signedUrl):
   nretry=4
   if signedUrl is None:
        return None
   filename=url_filename(signedUrl)
   for count in  range(1,nretry):
     try:
        getr=requests.get(signedUrl)
        getr.raise_for_status()
        break
        time.sleep(1)
     except requests.exceptions.RequestException as e:
        if count==nretry:
            print(e)
            return 1
   if getr.status_code==200:
        with open(filename, 'wb') as f:
             f.write(getr.content)
        lmStr=getr.headers["Last-Modified"]
        lmTime=utcStr_to_local_time(lmStr)
        os.utime(filename, (lmTime, lmTime))
        return 0
   else:
        print(getr.content)
        return 1


# filename can a patten with wild chars (like *.txt) to select multiple files
# the signedUrl argument can be used only with a single file
def download_project_file(username,password,folder,filename,signedUrl=None):
   nretry=4
   if signedUrl is not None:
      if(download_signed_url(signedUrl)==0):
          return [signedUrl]
   token =make_token(username,password)
   serverurl="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "get", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
     try:
        r=requests.get(serverurl, headers=headers, params=params )
        r.raise_for_status()
        break
        time.sleep(1)
     except requests.exceptions.RequestException as e:
        if count==nretry:
            print(e)
   if r.status_code !=200:
       print(r.content)
       r.raise_for_status()
   signedUrls=json.loads(r.content.decode("ascii"))
   for signedUrl in signedUrls:
       download_signed_url(signedUrl)
   return signedUrls


def submit_job(username,password,folder,filename,job):
   nretry=4
   token =make_token(username,password)
   serverurl="https://api.hierarchical-electromagnetics.com/MwModeler/submit_job"
   headers = { 'Authorization' : token}
   params={"filename": filename, "folder": folder, "job": job}
   for count in  range(1,nretry):
     try:
        r=requests.get(serverurl, headers=headers, params=params )
        r.raise_for_status()
        break
        time.sleep(1)
     except requests.exceptions.RequestException as e:
        if count==nretry:
            print(e)
   if r.status_code !=200:
       print(r.content)
       r.raise_for_status()
   response=json.loads(r.content.decode("ascii"))
   return response




