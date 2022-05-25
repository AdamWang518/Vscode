#%%
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
mushrooms = defaultdict(lambda: [])
def checkType(size, length):
    if size < 10: return '發財蘑菇'
    if size < 15: return '幸運蘑菇'
    if length > 15:
        return '發財蘑菇'
    return '神秘蘑菇'
for i in range(100):
    size = random.randint(1, 20)
    length = random.randint(1, 20)
    mushrooms['type'].append(checkType(size, length))
    mushrooms['size'].append(size)
    mushrooms['length'].append(length) 
data = pd.DataFrame.from_dict(mushrooms)
#print(data) 
# y = data['type']                   # 變出 y 資料
# X = data.drop(['type'], axis=1)    # 變出 X 資料，將 type 丟棄

print(X)
# print(y)

# X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
# model = DecisionTreeClassifier()
# model.fit(X_train, y_train)
# model.score(X_test, y_test)
# y_predict = model.predict(X_test)
# accuracy_score(y_test, y_predict)



# answers =  []    # 存放真正答案
# predicts = []    # 存放預測結果
# for i in range(100):
#     size = random.randint(1, 20)
#     length = random.randint(1, 20)
#     answers.append(checkType(size, length))
#     predicts.append(model.predict([[size, length]]))
# accuracy_score(answers, predicts)
# export_graphviz(model, out_file='mushrooms.dot',
#                 feature_names=['size', 'length'],
#                 class_names=model.classes_)
# resolution = 100
# dx = np.linspace(np.min(1), np.max(20), resolution)
# dy = np.linspace(np.min(1), np.max(20), resolution)
# dx, dy = np.meshgrid(dx, dy)
# Xc = np.c_[dx.flatten(), dy.flatten()]
# z = model.predict(Xc)


# # convert predict to number
# kls = list(model.classes_)
# z = np.array([kls.index(v) for v in z])
# z = z.reshape(dx.shape)
# plt.contourf(dx, dy, z, alpha=0.6)
# plt.xlabel('size')
# plt.ylabel('length')
# plt.show()




#%%