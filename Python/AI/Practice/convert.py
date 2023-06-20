from PIL import Image,ImageOps
import matplotlib.pyplot as plt
#自己画图的存放地址
picture=Image.open(r'3.png')
# print(picture)
# print(picture.mode)
# print(picture.getpixel((15,10)))
picture_L=picture.convert('L',)
# print(picture_L)
# print(picture_L.size)
# print(picture_L.getpixel((15,10)))
# print(picture_L.mode)
inverted_image=ImageOps.invert(picture_L)#因为画图软件是白底黑字，与MNIST相反，所以要反转一下
# plt.figure(figsize=(15,15))
# plt.tick_params(colors='white')#设置图片坐标刻度颜色
######生成的图像已经归一化，像素点数值在0-1之间
inverted_image.save(r'3_gray.png')#保存转化后的图像
plt.figure(figsize=(15,10))
plt.tick_params(colors='white')  #设置坐标刻度颜色
plt.imshow(inverted_image)  #显示要预测的瑞利图（默认显示）

