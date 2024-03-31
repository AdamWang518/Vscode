import concurrent.futures
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
import os
import re
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from tqdm import tqdm  # 引入 tqdm

def element_text_to_be_present_in_element(locator, text_):
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
    try:
        WebDriverWait(driver, 10).until(
            element_text_to_be_present_in_element(
                (By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > div > div > div:nth-child(1) > h1'), "會員專區"
            )
        )
    except :
        print("登錄後未找到預期的'會員專區'標題，請檢查登錄過程是否成功。")

def download_novel(novel_url, progress):
    service = Service("D:\\Github\\Vscode\\Python\\driver_source\\chromedriver.exe")
    options = webdriver.ChromeOptions()
    driver = webdriver.Chrome(service=service, options=options)
    login(driver)
    driver.get(novel_url.strip())

    try:
        name = driver.find_element(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div:nth-child(1) > div.col-md-9.book-detail > h2').text
        name = replace_special_chars(name)
        base_path = 'D:\\MEGA\\小說\\ESJ爬蟲\\'
        novel_path = os.path.join(base_path, f"{name}.txt")

        os.makedirs(os.path.dirname(novel_path), exist_ok=True)

        with open(novel_path, 'w', encoding='utf8') as file:
            file.write(f'小說連結: {novel_url}\n\n')

            chapter_elements = driver.find_elements(By.CSS_SELECTOR, '#chapterList > details > a')
            chapter_elements += driver.find_elements(By.CSS_SELECTOR, '#chapterList > details > details > a')
            chapter_elements += driver.find_elements(By.CSS_SELECTOR, '#chapterList >  a')
            chapters_urls = [element.get_attribute('href') for element in chapter_elements if 'esjzone.cc' in element.get_attribute('href')]

            for chapter_url in chapters_urls:
                driver.get(chapter_url)
                chapter_title = driver.find_element(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > h2').text
                chapter_content_elements = driver.find_elements(By.CSS_SELECTOR, 'body > div.offcanvas-wrapper > section > div > div.col-xl-9.col-lg-8.p-r-30 > div.forum-content.mt-3')
                chapter_content = ''.join([element.get_attribute('innerText') for element in chapter_content_elements])

                file.write(f"{chapter_title}\n\n")
                file.write(f"{chapter_content}\n\n")
                file.write(f"{'=' * 40}\n\n")
    except Exception as e:
        print(f"Error processing novel {novel_url}: {e}")
    finally:
        driver.quit()
        progress.update(1)  # 更新进度条

def main():
    with open('D:\\Github\\Vscode\\Python\\ESJ-download\\ESJ_List.txt', 'r', encoding='utf8') as listReader:
        novel_urls = listReader.readlines()

    progress = tqdm(total=len(novel_urls), desc='Downloading Novels')  # 初始化进度条
    with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
        # 使用futures的方式来提交下载任务，同时传递progress对象
        futures = [executor.submit(download_novel, novel_url.strip(), progress) for novel_url in novel_urls]
        # 等待所有的任务完成
        concurrent.futures.wait(futures)

    progress.close()  # 关闭进度条

if __name__ == "__main__":
    main()
