import sys
import json
from types import SimpleNamespace
if sys.version > '3' : raw_input = input  # Python 3

class Node :
    "Node objects have a question, and  left and right pointer to other Nodes"
    def __init__ (self, question, left=None, right=None) :
        self.question = question
        self.left     = left
        self.right    = right
    def toJSON(self):
        return json.dumps(self, default=lambda o: o.__dict__, 
            sort_keys=True, indent=4)

def yes (ques) :
    "The user answers 'yes' or something similar. Otherwise it's a no"
    while True :
        ans = raw_input (ques)
        ans = ans[0:1].lower()
        if ans == 'y' : return True
        else          : return False

knowledge = Node("資訊工程學系")

def main () :
    "Guess the animal. Add a new Node for a wrong guess."
    # f = open("Python\Akinator\data.json", "r")
    while True :
        f = open("Python\Akinator\data.json", "r") 
        if f.read()=='':
            knowledge = {}
            knowledge['question']='資工系'
            knowledge['left'] = None
            knowledge['right'] = None
            
        else:
            knowledge = json.load(open('Python\Akinator\data.json', 'r')) 
        print("")      # line break, either Python 2 or 3

        if not yes("你有想猜的科系嗎 ") : break
        p = knowledge
        while p.left != None :
            if yes(p.question+"? ") : p = p.right
            else                    : p = p.left
    
        if yes("它是 " + p.question + "? ") : continue
        department   = raw_input ("這個科系的名字是?")
        question = raw_input ("什麼樣的問題可以區分 %s 跟 %s? "
                                  % (department,p.question))
        p.left     = Node(p.question)
        p.right    = Node(department)
        p.question = question
    
        if not yes ("如果是 %s 的話答案會是? " % department) :
            (p.right, p.left) = (p.left, p.right)

        f = open("Python\Akinator\data.json", "w")
        f.write(knowledge.toJSON())
        f.close()  

if __name__ == "__main__" : main ()