import os
import re
import operator
import string
from zhon.hanzi import punctuation
import matplotlib.pyplot as plt   
cur_path=os.path.dirname(__file__) # 取得目前路徑
tree=os.walk(cur_path)
allfiles=[]
words=[]
punctuation_en=string.punctuation
punctuation_zh=punctuation
for dirname,subdir,files in tree:
    for file in files:  # 取得所有.py 檔，存入 allfiles 串列中
        ext=file.split('.')[-1]
        if ext=="py": 
            allfiles.append(dirname +'\\'+file)
if len(allfiles)>0:  
    for file in allfiles:  # 讀取 allfiles 串列所有檔案  
        try:
            fp = open(file, "r", encoding = 'UTF-8')
            content = fp.read()#取得檔案內文
            for en in punctuation_en:
                content=content.replace(en,',')#去除英文標點符號
            for zh in punctuation_zh:
                content=content.replace(zh,',')#去除中文標點符號
            words+=re.split(' |\n|\t|,',content)#去除空格及換行符號
        except:
            print("{} 無法讀取..." .format(file))
    while '' in words:
        words.remove('')#移除切割出的空字串
    total=0
    counts = dict() 
    for word in words:  
        counts[word] = counts.get(word,0) + 1#計算詞出現的個別次數和總次數
        total+=1
    os.system("cls")
    print("-------------------------")
    print("計算結果")
    print("-------------------------")
    print('總詞數為:'+str(total))
    sortedDict = dict(sorted(counts.items(), key=operator.itemgetter(1),reverse=True))#以次數由大到小排序dict
    print('次數統計結果:')
    print(sortedDict)
    for i in counts:
        counts[i]=counts[i]/total#轉換次數為詞頻(Term Frequency)
    sortedDict = dict(sorted(counts.items(), key=operator.itemgetter(1),reverse=True))#以詞頻由大到小排序dict
    print('詞頻統計結果:')
    print(sortedDict)
    keyList=[]
    itemList=[]
    for i in sortedDict:
        keyList.append(i)
        itemList.append(sortedDict[i])
    plt.bar(keyList[0:10],itemList[0:10])
    plt.legend()
    plt.xlabel("Words")
    plt.ylabel("Term Frequency")
    plt.show()      
    


