import concurrent.futures
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
import os
import re
import time
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
def element_text_to_be_present_in_element(locator, text_):
    """定義一個自定義等待條件，檢查元素的內文是否包含特定文字"""

    def predicate(driver):
        try:
            element_text = driver.find_element(*locator).text
            return text_ in element_text
        except Exception:
            return False

    return predicate
def replace_special_chars(string):
    pattern = r'[\/:?"<>|]'
    return re.sub(pattern, '_', string)
def login(driver):
    driver.get("https://www.esjzone.cc/my/login")
    WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.XPATH, '/html/body/div[3]/section/div/div[1]/form/div[1]/input'))).send_keys('dodoga518@gmail.com')
    driver.find_element(By.XPATH, '/html/body/div[3]/section/div/div[1]/form/div[2]/input').send_keys('dodoga518')
    driver.find_element(By.XPATH, '/html/body/div[3]/section/div/div[1]/form/div[4]/a').click()
    # 檢查登錄後頁面的特定元素內文是否為"會員專區"
    try:
        WebDriverWait(driver, 10).until(
            element_text_to_be_present_in_element(
                (By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > div > div > div:nth-child(1) > h1'), "會員專區"
            )
        )
    except :
        print("登錄後未找到預期的'會員專區'標題，請檢查登錄過程是否成功。")
def download_novel(novel_url):
    service = Service("D:\\Github\\Vscode\\Python\\driver_source\\chromedriver.exe")
    options = webdriver.ChromeOptions()
    driver = webdriver.Chrome(service=service, options=options)
    login(driver)
    driver.get(novel_url.strip())  # 確保URL沒有尾隨的空白字符

    try:
        # 假設以下選擇器能夠正確獲取小說名稱
        name = driver.find_element(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div:nth-child(1) > div.col-md-9.book-detail > h2').text
        name = replace_special_chars(name)
        base_path = 'D:\\MEGA\\小說\\ESJ爬蟲\\'
        novel_path = os.path.join(base_path, f"{name}.txt")
        
        # 確保目錄存在
        os.makedirs(os.path.dirname(novel_path), exist_ok=True)

        with open(novel_path, 'w', encoding='utf8') as file:
            file.write(f'小說連結: {novel_url}\n\n')

            # 獲取所有章節的連結
            chapter_elements = driver.find_elements(By.CSS_SELECTOR, '#chapterList > details > a')
            chapter_elements+=driver.find_elements(By.CSS_SELECTOR, '#chapterList > details > details > a')
            chapter_elements+=driver.find_elements(By.CSS_SELECTOR, '#chapterList >  a')
            chapters_urls = [element.get_attribute('href') for element in chapter_elements if 'esjzone.cc' in element.get_attribute('href')]
            
            for chapter_url in chapters_urls:
                driver.get(chapter_url)
                # 獲取章節標題和內容，以下選擇器需要根據實際網頁結構修改
                chapter_title = driver.find_element(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > h2').text
                chapter_content_elements = driver.find_elements(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div.forum-content.mt-3')
                chapter_content = '\n'.join([element.get_attribute('innerText') for element in chapter_content_elements])

                # 將章節標題和內容寫入檔案
                file.write(f"{chapter_title}\n\n")
                file.write(f"{chapter_content}\n\n")
                file.write(f"{'=' * 40}\n\n")  # 添加分隔符以區分不同章節

    except Exception as e:
        print(f"Error processing novel {novel_url}: {e}")
    finally:
        driver.quit()


def main():
    # 讀取包含所有小說連結的文件
    with open('D:\\Github\\Vscode\\Python\\ESJ-download\\ESJ_List.txt', 'r', encoding='utf8') as listReader:
        novel_urls = listReader.readlines()
    
    # 使用多線程來平行處理每個小說的爬取
    with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
        executor.map(download_novel, novel_urls)

if __name__ == "__main__":
    main()

    
