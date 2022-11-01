import os,ast
dictionary=dict()
cur_path=os.path.dirname(__file__)+'\\'
def menu():
    os.system("cls")
    print("-------------------------")
    print("字典系統")
    print("-------------------------")
def read_data():
    fp = open(cur_path+'dict_a.txt', "r",encoding = 'UTF-8')
    lines = fp.readlines()[1:]#捨棄第一行
    for line in lines:
        word=line.split(',')[0]
        translations=line.split(',')[3:]
        translation=""
        comma=""
        for trans in translations:
            if trans!="":
                translation+=comma+trans
            else:
                break
            if comma=="":
                comma=","
        #有些文字中可能會夾雜逗號，因此以空字串為分界線來進一步串接並加上逗號，否則apple就只能得出"n. 苹果而不是"n. 苹果, 家伙\n[医] 苹果"
        dictionary[word]=translation
def input_data():       
    while True:
        word =input("請輸入文字:")
        if word in dictionary:
            print("{}詞彙存在字典中".format(word))
            print(dictionary[word])
        else:
            print("{}詞彙不存在字典中".format(word))    
read_data()
menu()
input_data()