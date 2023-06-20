from firebase import firebase

url = 'https://newsdb-6da43-default-rtdb.firebaseio.com/'
fb = firebase.FirebaseApplication(url, None)
id=input("請輸入想改變的資料id")
news = fb.put('/news/'+id, 'title','test')
print("已進行標題修改")