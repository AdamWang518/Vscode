import pandas as pd
 
 
#所需的欄位
usecols = ['是否跟設計有關?', '是否跟管理有關?', '是否跟資訊有關?', '是否跟醫療有關?', '是否跟實體的產品有關?','是否屬於管理學院?','是否跟AI有關?','教授的男女比例是否大致相等?','學生的男女比例是否大致相等?','有沒有畢業專題?','有沒有交換學生計畫?']
df = pd.read_csv('Python\Akinator\question.csv', usecols=usecols) 
new_df = df.dropna()
print(new_df)