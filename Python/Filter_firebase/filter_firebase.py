import firebase_admin
from firebase_admin import credentials
import json

cred = credentials.Certificate("D:\\Vscode\\Python\\Filter_firebase\\firebase.json")
firebase_admin.initialize_app(cred)
db = firebase_admin.firestore.client()
f = open('D:\\Vscode\\Python\\Filter_firebase\\attraction.json', 'r',encoding='utf8')
inFo=json.loads(f.read())
Attractions=inFo["Info"]
for Attraction in Attractions:
    item=dict()
    item["Name"]=Attraction["Name"]
    item["Add"]=Attraction["Add"]
    item["Toldescribe"]=Attraction["Toldescribe"]
    item["Description"]=Attraction["Description"]
    item["Tel"]=Attraction["Tel"]  
    item["Region"]=Attraction["Region"] 
    item["Town"]=Attraction["Town"] 
    item["Picture1"]=Attraction["Picture1"] 
    item["Px"]=Attraction["Px"] 
    item["Py"]=Attraction["Py"] 
    item["Parkinginfo_Px"]=Attraction["Parkinginfo_Px"] 
    item["Parkinginfo_Py"]=Attraction["Parkinginfo_Py"]  
    item["Ticketinfo"]=Attraction["Ticketinfo"]
    doc_ref = db.collection("Attractions").document(Attraction["Id"])
    doc_ref.set(item)
f.close()
