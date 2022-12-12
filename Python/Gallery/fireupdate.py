import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
import json
cred = credentials.Certificate("D:\\Vscode\\Python\\Gallery\\gallery_key.json")
firebase_admin.initialize_app(cred)
db = firestore.client()
f = open('D:\\Vscode\\Python\\Gallery\\trimmed.json', 'r',encoding='utf8')
inFo=json.loads(f.read())
draws=inFo["inFo"]
#讀取json檔的全部名畫並上傳至firebase
for draw in draws:
    item=dict()
    item["imageLink"]=draw["imageLink"]
    item["drawName"]=draw["drawName"]
    item["artist"]=draw["artist"]
    item["Attribute"]=draw["Attribute"]
    item["startYear"]=draw["startYear"]
    item["endYear"]=draw["endYear"]
    doc_ref = db.collection("Gallery").document()
    doc_ref.set(item)
f.close()