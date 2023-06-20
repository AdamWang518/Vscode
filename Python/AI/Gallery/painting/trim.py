import json
f = open('D:\\Vscode\\Python\\SQLHW\\painting\\painting.json', 'r',encoding='utf8')
fend = open("D:\\Vscode\\Python\\SQLHW\\painting\\trimed.json", "w", encoding='utf8')
PrimeJson = json.load(f)
Data=[]
InFos=PrimeJson["inFo"]
infoDict=dict()
infoDict["inFo"]=[]
for Info in InFos:
    trimDict=dict()
    trimDict["imageLink"]=Info["imageLink"]
    trimDict["drawName"]=Info["drawName"]
    trimDict["artist"]=Info["artist"]
    if len((Info["year"]).split(' - '))==1:
        trimDict["startYear"]=str(Info["year"]).split(' - ')[0]
        trimDict["endYear"]=""
    else:
        trimDict["startYear"]=str(Info["year"]).split(' - ')[0]
        trimDict["endYear"]=str(Info["year"]).split(' - ')[1]
    trimDict["Attribute"]=Info["Attribute"]
    infoDict["inFo"].append(trimDict)
json.dump(infoDict, fend, ensure_ascii=False)
