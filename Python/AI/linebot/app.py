from flask import Flask, request, send_from_directory
import json
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, TextSendMessage, ImageSendMessage
import os
import time
import random

app = Flask(__name__)

access_token = 'yhyI/dK7CCtgenOBkXc7usOJL9aQX0V4GuslJIntsCq9GidwRafaXzwI+d6xSWATlE5x5tR8jiUo+OS+qeGheTSr1RvFN2TmBzhULTuXeUebRK41J8kL9yyG39Eipyein32OGrXAs8GsEDmiR3v7yQdB04t89/1O/w1cDnyilFU='
secret = '9ec06d24960fee412114422b26456354'
line_bot_api = LineBotApi(access_token)
handler = WebhookHandler(secret)

@app.route("/", methods=['POST'])
def linebot():
    body = request.get_data(as_text=True)  # 取得收到的訊息內容
    try:
        json_data = json.loads(body)  # json 格式化訊息內容
        signature = request.headers['X-Line-Signature']  # 加入回傳的 headers
        handler.handle(body, signature)  # 綁定訊息回傳的相關資訊
        event = json_data['events'][0]  # 取得第一個事件
        tk = event['replyToken']  # 取得回傳訊息的 Token
        message_type = event['message']['type']  # 取得 LINE 收到的訊息類型

        messages = []  # 保存回复消息的列表

        if message_type == 'text':
            msg = event['message']['text']  # 取得 LINE 收到的文字訊息
            reply = msg
            messages.append(TextSendMessage(text=reply))  # 添加文本消息到列表

        elif message_type == 'image':

            msg_id = event['message']['id']  # 取得訊息 id
            print(msg_id)
            message_content = line_bot_api.get_message_content(msg_id)  # 根據訊息 ID 取得訊息內容

            # Generate timestamp for the directory name
            timestamp = str(time.time()).replace('.', '')

            # Generate random style name
            styles = ['candy', 'udnie', 'rain', 'mosaic']

            # Create the four directories
            directories = []
            for style in styles:
                directory = f"./{timestamp}_{style}"
                print(style)
                directories.append(directory)
                if not os.path.exists(directory):
                    os.makedirs(directory)
            

            # Save the image file in each directory
            for directory in directories:
                print(directory)
                file_path = f'{directory}/input.jpg'
                with open(file_path, 'wb') as file:
                    file.write(message_content.content)

                reply = f'圖片儲存完成！風格：{directory}'
                print(reply)
                # messages.append(TextSendMessage(text=reply))  # 添加文本消息到列表

            print(f'{directory}/input.jpg')
            print(os.path.exists(f'{directory}/input.jpg'))
            time.sleep(100)

            for directory in directories:
                if os.path.exists(f'{directory}/output.jpg')==True:
                    print(f'{directory}/output.jpg')
                    print(os.path.exists(f'{directory}/output.jpg'))
                # Construct image URL
                    image_url = f"https://82e5-61-218-122-231.ngrok-free.app/{os.path.join(directory.lstrip('./'), 'input.jpg')}"

                    image_message = ImageSendMessage(
                        original_content_url=image_url,
                        preview_image_url=image_url
                    )
                
                    messages.append(image_message)


        else:
            reply = '你傳的不是文字或圖片呦～'
            messages.append(TextSendMessage(text=reply))  # 添加文本消息到列表

        print(reply)
        line_bot_api.reply_message(tk, messages)  # 一次性发送多条消息

    except:
        print(body)

    return 'OK'


@app.route("/<path:directory>/<filename>", methods=['GET'])
def get_image(directory, filename):
    directory = directory.strip("./")  # 移除開頭的 "./"
    print(directory,filename)
    return send_from_directory(directory, filename)




if __name__ == "__main__":
    app.run(port=5001)