"""import spreadsheet data from a cvs file and merge it into Variables spreadsheet"""
import os.path

doc = FreeCAD.ActiveDocument
filePath = App.ActiveDocument.FileName[:-6]+"_var.csv"
if not os.path.exists(filePath):
    FreeCAD.Console.PrintMessage("Not found : "+filePath+"\n")
    exit(0)


variablesSheet=App.ActiveDocument.getObjectsByLabel("Variables")[0];
if variablesSheet == None:
    variablesSheet = doc.addObject("Spreadsheet::Sheet","Variables")
    variablesSheet.label="Variables"
    variablesSheet.importFile(filePath)
    FreeCAD.ActiveDocument.recompute()
    FreeCAD.Console.PrintMessage("A new Variables Spreadsheet was created from" +filePath+"\n")
    exit(0)
elif variablesSheet.TypeId != "Spreadsheet::Sheet":
    FreeCAD.Console.PrintMessage("Existing Variables Object is not a Spreadsheet\n")
    exit(1)

tempSheet=App.ActiveDocument.getObject("tempSheet");
if tempSheet != None:
    doc.removeObject(tempSheet.Name)

tempSheet =doc.addObject("Spreadsheet::Sheet","tempSheet")
tempSheet.importFile(filePath)
doc.recompute()

blackList = ["ExpressionEngine","Label","Label2","Visibility","cells","columnWidths","rowHeights","PythonMode", "ShowCells", "TreeRank", "hiddenColumns", "hiddenRows", "ViewObject"]
tempSheetCells = [c for c in tempSheet.PropertiesList if c not in blackList]
cellsChanged = 0
doc.openTransaction("Merge Spreadsheets")
for cell in tempSheetCells:
    if getattr(variablesSheet,cell) != getattr(tempSheet,cell):
            FreeCAD.Console.PrintMessage("Merging "+tempSheet.Label+"."+cell+" to "+variablesSheet.Label+"."+cell+" --> "+str(getattr(tempSheet,cell))+"\n")
            variablesSheet.set(cell,str(getattr(tempSheet,cell)))
            cellsChanged += 1
FreeCAD.Console.PrintMessage("Done merging "+tempSheet.Label+" into "+variablesSheet.Label+"\n")
FreeCAD.Console.PrintMessage("Cells changed = "+str(cellsChanged)+"\n")
FreeCAD.ActiveDocument.recompute()
doc.removeObject(tempSheet.Name)
doc.commitTransaction()
