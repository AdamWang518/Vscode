from firebase import firebase
url = 'https://newsdb-6da43-default-rtdb.firebaseio.com/'
fb = firebase.FirebaseApplication(url, None)
news = fb.get('/news', None)
for key, value in news.items():
    print("key={},title={},category={},date={}".format(
        key, value["title"], value["category"], value["date"]))
