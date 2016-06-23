# -*- coding: utf-8 -*-
import numpy as np
import scipy as sp
from sklearn import tree
from sklearn.metrics import precision_recall_curve
from sklearn.metrics import classification_report
from sklearn.cross_validation import train_test_split
from sklearn.externals.six import StringIO
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.externals.joblib import Parallel, delayed
import time
import pydot
import sys
import platform
#from sklearn.tree import export_graphviz
#from multiprocessing import cpu_count
#print "cpu_count:",(cpu_count())

from IPython.display import Image
def dt_run(train_dir,test_dir,flag,tree_type,samsplit,samleaf,weight0,weight1,num):
        count=0
        print "random forest:"
        print "train_data:",train_dir
        print "test_data:",test_dir
        print "program begin"
        start = time.clock()
        #gc.disable()
        ''' 数据读入 '''
        data  = [[0.0]*87]*1300000
        #labels  = [0.0]*1300000
        #data  = [[0.0]*87]*2000000
        labels = ['0']*1300000
        with open(train_dir) as ifile:
                for line in ifile:
                        tokens = line.strip().split(',')
                        #data.append([float(tk) for tk in tokens[4:-1]])
                        #labels.append(tokens[-1])
                        data[count]=[float(tk) for tk in tokens[4:-1]]
                        labels[count]=tokens[-1]
                        del tokens
                        count=count+1
                        if (count%200000==0):
                                print count
        #print data[0]
        data=data[:count]
        print len(data)
        labels=labels[:count]
        x_train = np.array(data)
        labels = np.array(labels)
        y_train = np.zeros(labels.shape)

        y_train[labels=='1']=1
        end = time.clock()
        print "train_data read!!!,used time:",end-start
        start = time.clock()
        #gc.enable()
        #gc.disable()
        del data
        del labels
        count=0
        data  = [[0.0]*87]*1300000
        labels = ['0']*1300000#labels  = []
        with open(test_dir) as ifile:
                for line in ifile:
                        tokens = line.strip().split(',')
                        #data.append([float(tk) for tk in tokens[4:-1]])
                        #labels.append(tokens[-1])
                        data[count]=[float(tk) for tk in tokens[4:-1]]
                        labels[count]=tokens[-1]
                        count=count+1
                        if (count%200000==0):
                                print count
        data=data[:count]
        labels=labels[:count]
        print len(data)
        x_test = np.array(data)
        labels = np.array(labels)
        y_test = np.zeros(labels.shape)
        #print x_test.size/87
        #print y_test.size
        #print x_test[1];
        ''' 标签转换为0/1 '''

        y_test[labels=='1']=1
        #gc.enable()
        ''' 拆分训练数据与测试数据 '''
        #x_train, x_test, y_train, y_test = train_test_split(x, y, test_size = 0.2)
        end = time.clock()
        print "test_data read!!!,used time:",end-start
        start = time.clock()
        ''' 使用信息熵作为划分标准，对决策树进行训练 '''
        clf = RandomForestClassifier(n_estimators=num,criterion=tree_type,min_samples_split=samsplit,min_samples_leaf=samleaf,class_weight={0:weight0,1:weight1},n_jobs=-1)
        print(clf)
        clf.fit(x_train, y_train)

        end = time.clock()
        print "train over !!!,used time:",end-start
        start = time.clock()
        
        ''' 系数反映每个特征的影响力。越大表示该特征在分类中起到的作用越大 '''
        #print(clf.feature_importances_)

        '''测试结果的打印'''
        #answer = clf.predict(x_train)
        #print(x_train)
        #print(answer)
        #print(y_train)
        #print(np.mean( answer == y_train))

        a=0
        b=0
        c=0
        d=0
        '''准确率与召回率'''
        precision, recall, thresholds = precision_recall_curve(y_test, clf.predict(x_test))
        
        answer = clf.predict_proba(x_test)[:,1]
        for i in xrange(0,answer.size):
                
                if(answer[i]<=0.5):
                        answer[i]=0
                else:
                        answer[i]=1
                
                #print answer[i],y_test[i]
                if (answer[i]==0 and y_test[i]==0):
                                        d=d+1 
                elif (answer[i]==0 and y_test[i]==1):
                                        c=c+1 
                elif (answer[i]==1 and y_test[i]==0):
                                        b=b+1 
                elif (answer[i]==1 and y_test[i]==1):
                                        a=a+1
                else:
                        print answer[i],y_test[i]
        #print answer
        #print(classification_report(y_test, answer, target_names = ['online', 'offline']))

        end = time.clock()
        print "test over!!!,used time:",end-start
        start = time.clock()
        print "测试集测试结果:"#,answer.size
        print "predict offline and offline in fact:",a
        print "predict offline but online in fact:",b
        print "predict online but offline in fact:",c
        print "predict online and online in fact:",d
        print "offline_rate:",float(100*(a+c)/(a+b+c+d)),"%"
        print "data size",a+b+c+d
        print "precision_rate:",float(100*a/(a+b)),"%"
        print "recall_rate:",float(100*a/(a+c)),"%"
        if (flag==1):
                ''' 把决策树结构写入文件 '''
                #with open("tree.dot", 'w') as f:
                #        f = tree.export_graphviz(clf, out_file=f)
                #dot_data = StringIO()
                #tree.export_graphviz(clf, out_file=dot_data)
                #graph = pydot.graph_from_dot_data(dot_data.getvalue())
                #graph.write_pdf("tree.pdf")
                for i in xrange(len(clf.estimators_)):
                        dot_data=StringIO()
                        tree.export_graphviz(clf.estimators_[i], out_file=dot_data)
                        graph = pydot.graph_from_dot_data(dot_data.getvalue())
                        graph.write_pdf('tree%d.pdf'%i)
                        print 'tree%d.pdf print over'%i
        print "program end"

if (platform.system()=='Windows'):
        dt_run('ZDYCL_01_11_PUB_8_2.csv','ZDYCL_02_12_PUB_8_2.csv',1,'entropy',100,50,0.52,1,5)
elif (platform.system()=='Linux'):
        try:
                dt_run(sys.argv[1],sys.argv[2],int(sys.argv[3]),sys.argv[4],int(sys.argv[5]),int(sys.argv[6]),float(sys.argv[7]),float(sys.argv[8]),int(sys.argv[9]))
                
        except:
                print "error!!Check your argv and run the program again please:"
                print "argv[1] is the file name of train_data"
                print "argv[2] is the file name of test_data"
                print "argv[3] is the tag that whether we need to print the tree? 1 stand for yes,0 for no"
                print "argv[4] is the type of criterion: gini or entropy"
                print "argv[5] is the min samples number of a non-leaf Node"
                print "argv[6] is the min samples number of a leaf Node"
                print "argv[7] is the weight of class 0 which could be an int or float"
                print "argv[8] is the weight of class 1 which could be an int or float"
                print "argv[9] is the number(or size) of the forest"
                
#dt_run('ZDYCL_01_11_PUB_8_2.csv','ZDYCL_02_12_PUB_8_2.csv',0,'entropy',100,50,0.52,1,100)
#dt_run(sys.argv[1],sys.argv[2],int(sys.argv[3]),sys.argv[4],int(sys.argv[5]),int(sys.argv[6]),float(sys.argv[7]),float(sys.argv[8]),int(sys.argv[9]))


