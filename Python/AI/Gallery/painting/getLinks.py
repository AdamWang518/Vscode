
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
pageList = []
styles = []
drawArray = []
f = open("D:\\Vscode\\Python\\SQLHW\\painting\\links.text", "w", encoding='utf8')
gallery = "https://www.ss.net.tw/index.php?view=artist"
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get(gallery)
#先前往首頁
for i in range(1, 28):
    styles.append(str(driver.find_element(By.CSS_SELECTOR, "#A"+str(i) +
                  " > div > a:nth-child(1)").get_attribute("href")).replace("#title", ""))
#取得每個畫風分類的第一個分區連結，其中包含有該畫風所有的畫，同時移除#title方便等下操作網址

for style in styles:
    # 遍歷所有要爬的畫風
    driver.get(style)
    lastPage = int(str(driver.find_elements(
        By.CLASS_NAME, 'page-link')[-1].get_attribute('href')).split('~')[-1])
    #取得該畫風的最後一頁連結以計算要爬的頁數
    for i in range(1, lastPage+1):
        pagelink = style+"~"+str(i)
        #逐漸累加頁數讓每頁都能被爬到
        driver.get(pagelink)
        # pageDict=dict()
        drawlinks = driver.find_elements(
            By.CSS_SELECTOR, 'div.row.row-cols-2.row-cols-sm-3.row-cols-md-4.row-cols-lg-5.row-cols-xl-6.align-items-end > div > div > a')
        #取得該頁的所有名畫頁面連結
        for link in drawlinks:
            pageList.append(link.get_attribute('href'))
            f.write(link.get_attribute('href')+"\n")
            #將名畫頁面連結存入txt檔案，方便接下來爬蟲
f.close()
print('結束')



