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
data=defaultdict(lambda: [])
question=defaultdict(lambda: [])
def IndustryType():
    for i in range(0,100):
        data['是否跟醫療有關'].append(random.randint(1, 2))
        data['是否跟資訊有關'].append(random.randint(4, 5))
        data['是否跟專案管理有關'].append(random.randint(1,2))
        data['教授的男女比例是否大致相等'].append(random.randint(1,2))
        data['學生的男女比例是否大致相等'].append(random.randint(1,2))
        data['type'].append('工學院')
    return 
def ManageType():
  
    for i in range(0,100):
        data['是否跟醫療有關'].append(random.randint(1, 3))
        data['是否跟資訊有關'].append(random.randint(1, 3))
        data['是否跟專案管理有關'].append(random.randint(3, 5))
        data['教授的男女比例是否大致相等'].append(random.randint(4, 5))
        data['學生的男女比例是否大致相等'].append(random.randint(4, 5))
        data['type'].append('管理學院')
    return 
def MedicalType():
    
    for i in range(0,100):
        data['是否跟醫療有關'].append(random.randint(4, 5))
        data['是否跟資訊有關'].append(random.randint(1, 2))
        data['是否跟專案管理有關'].append(random.randint(1, 2))
        data['教授的男女比例是否大致相等'].append(random.randint(3, 5))
        data['學生的男女比例是否大致相等'].append(random.randint(3, 5))
        data['type'].append('醫學院')
    return 
def Test():
    print("請以1到5的數字回答以下問題(1:否 2:好像不是 3:不清楚 4:好像是 5:是)")
    question['是否跟醫療有關'].append(int(input("是否跟醫療有關")))
    question['是否跟資訊有關'].append(int(input('是否跟資訊有關')))
    question['是否跟專案管理有關'].append(int(input('是否跟專案管理有關')))
    question['教授的男女比例是否大致相等'].append(int(input('教授的男女比例是否大致相等')))
    question['學生的男女比例是否大致相等'].append(int((input('學生的男女比例是否大致相等'))))
    return
IndustryType()
ManageType()
MedicalType()
#print(data)
data = pd.DataFrame.from_dict(data)
#print(data) 
y = data['type']                   # 變出 y 資料
X = data.drop(['type'], axis=1)    # 變出 X 資料，將 type 丟棄
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
model = DecisionTreeClassifier()#建立決策樹
model.fit(X_train, y_train)#進行
Test()
question = pd.DataFrame.from_dict(question)
y_predict = model.predict(question)
print(y_predict)


