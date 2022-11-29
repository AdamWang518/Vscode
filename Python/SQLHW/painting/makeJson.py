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
        drawDict["drawName"] = driver.find_element(
            By.CSS_SELECTOR, '#title > div.col-md-5.col-lg-6 > img').get_attribute("alt")
        drawDict["artist"] = driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li.d-flex > span").text
        drawDict["year"] = str(driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li:nth-child(3)").text).replace("年\u3000\u3000份", "")
        drawDict["Attribute"] = driver.find_element(
            By.CSS_SELECTOR, "#productsInfo > ul > li:nth-child(5)").text.replace("原作材質", "")
        infoDict["inFo"].append(drawDict) 
    except:
        print(page)
        # 若有特定頁面有問題則將其吐出
json.dump(infoDict, f, ensure_ascii=False)
f.close()
