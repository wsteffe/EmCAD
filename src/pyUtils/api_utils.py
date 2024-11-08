import os,sys,time,datetime
from dateutil import tz
from base64 import b64encode
import requests
import json
import api_proxy
from pypac import PACSession, pac_context_for_url


def make_token(username,password):
   nameAndPass=username+":"+password
#token is base 64 encoded ant then decoded to acsii as python 3 stores it as a byte string
   token=b64encode(nameAndPass.encode('utf-8')).decode("ascii")
   return token


def get_file_headers(token,folder,filename):
   nretry=4
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "head", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
       try:
          with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
             r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
          r.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error requesting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
             return None
   fileurl=r.content.decode("ascii")
   for count in  range(1,nretry):
       try:
          with pac_context_for_url(fileurl, proxy_auth=None, pac=api_proxy.proxyPac):
            headr=requests.head(fileurl)
          headr.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error getting headers of {fileurl}: {e}", file=sys.stderr)
          if count==nretry-1:
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


def upload_project_file_with_put(token,filename,folder):
   global  proxyPac
   nretry=4
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "put", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
       try:
          with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
            r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
          r.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error requesting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
            raise
   for count in  range(1,nretry):
       try:
          with open(filename, 'rb') as data:
            with pac_context_for_url(fileurl, proxy_auth=None, pac=api_proxy.proxyPac):
              putr=requests.put(fileurl, data=data, verify=True)
            putr.raise_for_status()
            break
       except requests.RequestException as e:
          print(f"Error putting data into {fileurl}: {e}", file=sys.stderr)
          if count==nretry-1:
             raise


def upload_project_file(token,filename,folder, only_if_newer=False):
   global  proxyPac
   nretry=4
   if only_if_newer :
       headers=get_file_headers(token,folder,filename)
       if not local_file_is_newer(filename,headers):
           if not remote_file_is_expiring(headers):
                return
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   length=int(os.path.getsize(filename))
   params={"method": "post", "filename": filename, "folder": folder, "content-length": length}
   for count in  range(1,nretry):
       try:
          with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
             r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
          r.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error requesting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
             raise
   content=json.loads(r.content.decode("ascii"))
   url=content["url"]
   for count in  range(1,nretry):
       try:
          with pac_context_for_url(url, proxy_auth=None, pac=api_proxy.proxyPac):
             postr=requests.post(url, data=content["fields"],files={'file': open(filename, 'rb')}, verify=True)
          postr.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error posting data into {url}: {e}", file=sys.stderr)
          if count==nretry-1:
             raise


def url_filename(fileurl):
   fname_parts=fileurl.split('?')[0].split('/')
   l=len(fname_parts)
   fname=fname_parts[l-1]
   return fname


def download_signed_url(signedUrl):
    global  proxyPac
    nretry=4
    if signedUrl is None:
        return None
    filename=url_filename(signedUrl)
    for count in  range(1,nretry):
       try:
          with pac_context_for_url(signedUrl, proxy_auth=None, pac=api_proxy.proxyPac):
            getr=requests.get(signedUrl, verify=True)
          getr.raise_for_status()
          break
       except requests.RequestException as e:
          print(f"Error getting data from {signedUrl}: {e}", file=sys.stderr)
          if count==nretry-1:
             return 1
    with open(filename, 'wb') as f:
        f.write(getr.content)
    lmStr=getr.headers["Last-Modified"]
    lmTime=utcStr_to_local_time(lmStr)
    os.utime(filename, (lmTime, lmTime))
    return 0


# filename can a patten with wild chars (like *.txt) to select multiple files
# the signedUrl argument can be used only with a single file
def download_project_file(token,folder,filename,signedUrl=None):
   global  proxyPac
   nretry=4
   if signedUrl is not None:
      if(download_signed_url(signedUrl)==0):
          return [signedUrl]
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/signed_url"
   headers = { 'Authorization' : token}
   params={"method": "get", "filename": filename, "folder": folder}
   for count in  range(1,nretry):
      try:
         with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
            r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
         r.raise_for_status()
         break
      except requests.RequestException as e:
          print(f"Error getting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
            return None
   signedUrls=json.loads(r.content.decode("ascii"))
   for signedUrl in signedUrls:
       download_signed_url(signedUrl)
   return signedUrls



def submit_job(token,folder,filename,job):
   global  proxyPac
   nretry=4
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/submit_job"
   headers = { 'Authorization' : token}
   params={"filename": filename, "folder": folder, "job": job}
   for count in  range(1,nretry):
      try:
         with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
           r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
         r.raise_for_status()
         break
      except requests.RequestException as e:
          print(f"Error getting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
            return None
   response=json.loads(r.content.decode("ascii"))
   return response


def get_credit(token):
   nretry=4
   api_modeler_url="https://api.hierarchical-electromagnetics.com/MwModeler/credit"
   headers = { 'Authorization' : token}
   params={"method": "get"}
   for count in  range(1,nretry):
      try:
         with pac_context_for_url(api_modeler_url, proxy_auth=None, pac=api_proxy.proxyPac):
           r=requests.get(api_modeler_url, headers=headers, params=params, verify=api_proxy.verify )
         r.raise_for_status()
         if r.status_code !=200:
           print(r.content, file=sys.stderr)
         break
      except requests.RequestException as e:
          print(f"Error getting data from {api_modeler_url}: {e}", file=sys.stderr)
          if count==nretry-1:
            return None
   credit=json.loads(r.content.decode())["credit"]
   return credit



