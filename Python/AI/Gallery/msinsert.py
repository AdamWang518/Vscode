import json
import pyodbc
# server  資料庫伺服器名稱或IP
# user   使用者名稱
# password 密碼
# database 資料庫名稱
f = open('D:\\Vscode\\Python\\Gallery\\trimmed.json', 'r',encoding='utf8')

server = 'localhost\MSSQLSERVER01'
database = 'Gallery'
username = 'dodoga518'
password = '20010518'
# 建立MSSQL資料庫連接
conn = pyodbc.connect('DRIVER={ODBC Driver 18 for SQL Server};SERVER='+server+';DATABASE='+database+';'+'TrustServerCertificate=yes;'+'UID='+username+';PWD='+ password)
cursor = conn.cursor() 
inFo=json.loads(f.read())
draws=inFo["inFo"]
for draw in draws:
    #塞入資料庫
    cursor.execute("INSERT gallery_table (imageLink, drawName, artist, attribute, startYear,endYear) VALUES ('{}','{}','{}','{}','{}','{}')".format(draw["imageLink"],draw["drawName"],draw["artist"],draw["Attribute"],draw["startYear"],draw["endYear"]))
    conn.commit()
conn.close()