from firebase import firebase

url = 'https://newsdb-6da43-default-rtdb.firebaseio.com/'
fb = firebase.FirebaseApplication(url, None)
id=input("請輸入想刪除的資料id")
news = fb.delete('/news/'+id, None)
print("已進行刪除")

