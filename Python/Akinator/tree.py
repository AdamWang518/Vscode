import sys
if sys.version > '3' : raw_input = input  # Python 3
class Node :
    "Node objects have a question, and  left and right pointer to other Nodes"
    def __init__ (self, question, left=None, right=None,entropy=0) :
        self.question = question
        self.left     = left
        self.right    = right
        self.entropy    = entropy

def yes (ques) :
    "The user answers 'yes' or something similar. Otherwise it's a no"
    while True :
        ans = raw_input (ques)
        ans = ans[0:1].lower()
        if ans == 'y' : return True
        else          : return False

knowledge = Node("資工系")

def main () :
    "Guess the animal. Add a new Node for a wrong guess."

    while True :
        print("")      # line break, either Python 2 or 3
        if not yes("你有想猜的科系嗎 ") : break
        p = knowledge
        while p.left != None :
            if yes(p.question+"? ") : p = p.right
            else                    : p = p.left
    
        if yes("它是 " + p.question + "? ") : continue
        animal   = raw_input ("這個科系的名字是 ")
        question = raw_input ("什麼樣的問題可以區分 %s 跟 %s? "
                                  % (animal,p.question))
        p.left     = Node(p.question)
        p.right    = Node(animal)
        p.question = question

        
    
        if not yes ("如果是 %s 的話答案會是? " % animal) :
            (p.right, p.left) = (p.left, p.right)

if __name__ == "__main__" : main ()