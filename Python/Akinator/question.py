import csv
import pandas as pd
from sklearn.model_selection import train_test_split
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from collections import defaultdict
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import accuracy_score
from sklearn.tree import export_graphviz
import matplotlib.pyplot as plt
y=[]
usecols = ['是否跟設計有關?', '是否跟管理有關?', '是否跟資訊有關?', '是否跟醫療有關?', '是否跟實體的產品有關?','是否屬於管理學院?','是否跟AI有關?','教授的男女比例是否大致相等?','學生的男女比例是否大致相等?','有沒有畢業專題?','有沒有交換學生計畫?']
df = pd.read_csv('Python\Akinator\question.csv', usecols=usecols) 
new_df = df.dropna()
with open('Python\Akinator\\answer.csv',encoding="utf-8") as csvfile:
    rows = csv.reader(csvfile)
  # 以迴圈輸出每一列
    for row in rows:
        y.append(row[0]) 
X_train, X_test, y_train, y_test = train_test_split(new_df, y, test_size=0.2)
model = DecisionTreeClassifier()
model.fit(X_train, y_train)
model.score(X_test, y_test)
y_predict = model.predict(X_test)
print(accuracy_score(y_test, y_predict))



