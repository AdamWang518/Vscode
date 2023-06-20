import json
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
f = open("D:\\Vscode\\Python\\SQLHW\\painting\\links.text", "r", encoding='utf8')
pageList = []
line = f.readline()
while line:
    pageList.append(line)
    line = f.readline()
f.close()
f = open("D:\\Vscode\\Python\\SQLHW\\painting\\draws.json", "w", encoding='utf8')
infoDict=dict()
infoDict["inFo"]=[]
for page in pageList:
    driver.get(page)
    drawDict = dict()
    try:
        drawDict["imageLink"] = str(driver.find_element(
            By.CSS_SELECTOR, '#title > div.col-md-5.col-lg-6 > img').get_attribute("src")).replace("sthumb_images", "popup_images")
            # 將圖片連結從低畫質縮圖替換成高清版
        drawDict["drawName"] = driver.find_element(
            By.CSS_SELECTOR, '#title > div.col-md-5.col-lg-6 > img').get_attribute("alt")
            #取得圖畫名
        drawDict["artist"] = driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li.d-flex > span").text
            #取得作者名
        drawDict["year"] = str(driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li:nth-child(3)").text).replace("年\u3000\u3000份", "")
            # 去除不需要的文字
        drawDict["Attribute"] = driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li:nth-child(5)").text.replace("原作材質", "")
            # 去除不需要的文字
        infoDict["inFo"].append(drawDict) 
            # 打包成一個Dict
    except:
        print(page)
        # 若有特定頁面有問題則將其吐出，在之後再爬
json.dump(infoDict, f, ensure_ascii=False)
#製成json，方便之後讀取
f.close()
