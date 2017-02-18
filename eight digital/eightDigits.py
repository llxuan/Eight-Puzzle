#coding=utf-8
import numpy as np
import copy

#8数码算法的实现
#by:李凌璇

#空白快移动方向
direction=["left","up","right","down"]
#与之相对的移动方向
reDirection=["right","down","left","up"]

#求解当前八数码与目标状态的曼哈顿距离
def manhattan(data,target):
    manDist=0
    for i in range(len(data)):
        for j in range(len(data)):
            #0表示空格，不记录空格的曼哈顿距离
            if data[i][j]!=0 and data[i][j]!=target[i][j]:
                #当二者相应位置上的元素不相等时，计算曼哈顿距离
                #找到该数在目标状态的位置
                for m in range(len(target)):
                    for n in range(len(target)):
                        if data[i][j]==target[m][n]:
                            manDist=manDist+abs(m-i)+abs(n-j)
    #print(manDist)
    return manDist

#判断输入的序列是否可解-----只在维数为3的时候有效
#两个状态之间是否可达，可以通过计算两者的逆序值，若两者奇偶性相同，则可达
#否则两个状态不可达
#因为将空格进行上下左右移动时，并不会改变数字序列的奇偶性
def canSolve(data,target):
    #若输入序列与目标序列二者维度不同，直接退出
    if len(data)!=len(target)*len(target):
        return False
    #目标状态的逆序数为0
    #只有当输入序列的逆序数为偶数时，八数码问题才可解
    #初始化逆序数为0
    reverseNum=0
    for i in range(len(data)):
        #因为0表示空格，所以不记录其逆序数
        if(data[i]!=0):
            for j in range(i):
                #当位于i之前的数比它本身大时，逆序数加一
                if(data[j]>data[i]):
                    reverseNum=reverseNum+1
    #当该序列的逆序数为奇数时，问题不可解
    if reverseNum%2:
        return False
    #当该序列的逆序数为偶数时，该问题可解
    else :
        return True

#根据指定方向移动空白块————扩展该节点
def move(data,way):
    row=-1
    col=-1
    datamove=0
    num=len(data)
    newdata=copy.deepcopy(data)     #必须是深拷贝，否则newdata与data将指向同一地址   
    #找到空白块所在位置
    for i in range(num):
        for j in range(num):
            if data[i][j]==0:
                row=i
                col=j
    #根据指定方向移动空白块
    if way=="left" and col>0:
        newdata[row][col]=data[row][col-1]
        newdata[row][col-1]=0
        datamove=data[row][col-1]
    if way=="up" and row>0:
        newdata[row][col]=data[row-1][col]
        newdata[row-1][col]=0    
        datamove=data[row-1][col]
    if way=="down" and row<num-1:
        datamove=data[row+1][col]        
        newdata[row][col]=data[row+1][col]
        newdata[row+1][col]=0   
    if way=="right" and col<num-1:
        newdata[row][col]=data[row][col+1]
        newdata[row][col+1]=0
        datamove=data[row][col+1]
    return newdata,datamove
#使用IDA*算法求出最佳搜索路线
def IDAStar(data,target,depth,score,reDir,pathLimit,path):
    num=len(data)
    parameter=num
    if num==5:
        parameter=6
    #若当前状态与目标状态相同，说明搜索成功
    if ((np.array(data)==np.array(target)).all()):
        for i in range(len(path)):
            if i >= depth:
                path.pop(i)        
        #printOut(path)
        return True
    if depth>pathLimit:
        for i in range(len(path)):
            if i >= depth:
                path.pop(i)        
        return False
    for i in range(len(direction)):
        #若当前的移动方向与之前的移动方向相反，即空白块又移回了之前的状态
        if direction[i]==reDir:
            continue;           #则不考虑该方向的移动
        #按direction[i]指定的方向移动空白块
        nextData,datamove=move(data, direction[i])        
        if(data!=nextData and data!=target):
            nextscore=depth+parameter*manhattan(nextData,target)
            #若当前得分已经超出界限，则该路线不正确，剪枝
            if  nextscore>pathLimit:    
                continue;             
            path[depth]=datamove
            #print(data,nextData)
            
            if(IDAStar(nextData, target, depth+1, nextscore, reDirection[i], pathLimit,path)):                   
                return True
    return False
#输出搜索路线
def printOut(path):
    print("移动的格子：")
    print(path)
    return


def solveDigit(start):
    target=[]
    line=[]
    output=[]
    path={}
    num=len(start)
    parameter=num
    if num==5:
        parameter=6
    count=1
    for i in range(num):
        line=[]
        for j in range(num):
            line.append(count)
            count=(count+1)%(num*num)
        target.append(line)
        
    limit=parameter*manhattan(start,target)
    
    #print(target)
    while(not IDAStar(start,target,0,0," ",limit,path)):
        limit=limit+1
    
    for (k,v) in path.items():
        output.append(v) 
    print(output)
    return output

data1=[[1,0],[3,2]]
data2=[[3,0,6],[1,5,4],[7,2,8]]
data3=[[1,6,2,4],[5,10,3,8],[9,7,0,12],[13,14,11,15]]

solveDigit(data1)
solveDigit(data2)
solveDigit(data3)

