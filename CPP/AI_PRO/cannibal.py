import queue
import numpy as np
import prettytable as pt

M = int(input("请输入传教士的人数："))  # 传教士
C = int(input("请输入野人的人数:"))  # 野人
K = int(input("请输入船的最大容量："))


class Node:
    def __init__(self, m, c, b, parent, step):
        self.data = np.array([m, c, b])
        self.m = m  # m:左岸传教士数量
        self.c = c  # c:左岸野人数量
        self.b = b  # b:船的状态 b=1->船在左岸;b=0->船在右岸
        self.step = step
        self.f_loss = self.step + m + c - K * b  # f(n)
        self.parent = parent


def sorte_by_floss():
    tmp_open = opened.queue.copy()
    length = len(tmp_open)
    # 排序,从小到大,当一样的时候按照step的大小排序
    for i in range(length):
        for j in range(length):
            if tmp_open[i].f_loss < tmp_open[j].f_loss:
                tmp = tmp_open[i]
                tmp_open[i] = tmp_open[j]
                tmp_open[j] = tmp
            if tmp_open[i].f_loss == tmp_open[j].f_loss:
                if tmp_open[i].step > tmp_open[j].step:
                    tmp = tmp_open[i]
                    tmp_open[i] = tmp_open[j]
                    tmp_open[j] = tmp
    opened.queue = tmp_open
    

'算法'
opened = queue.Queue()  # open表
start_node = Node(M, C, 1, None, 0)  # 初始状态节点
end_data = np.array([0, 0, 0])
opened.put(start_node)
closed = {}


def refresh_open(now_node):
    '''
    :param now_node: 当前的节点
    :return:
    '''
    tmp_open = opened.queue.copy()  # 复制一份open表的内容
    for i in range(len(tmp_open)):
        '''这里要比较一下node和now_node的区别,并决定是否更新'''
        data = tmp_open[i]
        now_data = now_node.data
        if (data == now_data).all():
            data_f_loss = tmp_open[i].f_loss
            now_data_f_loss = now_node.f_loss
            if data_f_loss <= now_data_f_loss:
                return False
            else:
                print('')
                tmp_open[i] = now_node
                opened.queue = tmp_open  # 更新之后的open表还原
                return True
    tmp_open.append(now_node)
    opened.queue = tmp_open  # 更新之后的open表还原
    return True


def data_to_int(num):
    value = 0
    for i in num:
        value = value * 10 + i
    return value


def is_safe(node):
    '判断当前的节点 两岸是否安全'
    m, c, b = node.data
    if m < c and m != 0:  # 左岸的野人人数不得超过传教士,除非传教士为0
        return False
    if M - m < C - c and m != M:  # 右岸的野人人数不得超过传教士,除非传教士为0
        return False
    if m < 0 or c < 0:  # 人数不得小于0
        return False
    if m >= c or M - m >= C - c:
        return True


def method_a_function():
    while len(opened.queue) != 0:
        node = opened.get()
        if (node.data == end_data).all():
            return node
        closed[data_to_int(node.data)] = 1  # 取出的点加入closed表中
        for i in range(M + 1):  # 上船传教士
            for j in range(C + 1):  # 上船野人
                # 船上非法情况
                if i + j == 0 or i + j > K or (i != 0 and i < j):
                    continue
                # a=a+1
                if node.b == 1:  # 当前船在左岸，下一状态统计船在右岸的情况
                    child_node = Node(node.m - i, node.c - j, 0, node, node.step + 1)
                else:  # 当前船在右岸，下一状态统计船在左岸的情况
                    child_node = Node(node.m + i, node.c + j, 1, node, node.step + 1)
                if data_to_int(child_node.data) not in closed:
                    # 更新一下open表,如果当前的状态已经在open表,选择性添加一下
                    if is_safe(child_node.data):
                        refresh_open(child_node)
        '''为open表进行排序,根据其中的f_loss值'''
        sorte_by_floss()
    return None


result_node = method_a_function()


def output_result(node):
    all_node = [node]
    for i in range(node.step):
        father_node = node.parent
        all_node.append(father_node)
        node = father_node
    return list(reversed(all_node))


def cal_tb_row(node, option):
    data = node.data
    m, c, b = data
    return [node.step, m, c, option[node.step], M - m, C - c]


def cal_option(node_list):
    option = []
    for i in range(len(node_list) - 1):
        now_m, now_c, now_b = node_list[i].data
        next_m, next_c, next_b = node_list[i + 1].data
        mr = abs(next_m - now_m)
        cr = abs(next_c - now_c)
        mr = f'{mr} missionary' if mr else ''
        cr = f'{cr} cannibals ' if cr else ''
        status = 'turn right,' + mr + ' ' + cr if now_b == 1 else 'turn left,' + mr + ' ' + cr
        option.append(status)
    option.append('success!')
    return option


def out_put():
    node_list = output_result(method_a_function())
    option = cal_option(node_list)
    tb = pt.PrettyTable()
    field_names = ['step', 'm_left', 'c_left', 'option', 'm_right', 'c_right']
    tb.field_names = field_names
    for node in node_list:
        tb.add_row(cal_tb_row(node, option))
    print(tb)


out_put()