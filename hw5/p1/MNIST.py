# EE569 Homework Assignment #5 Sample Code
# requirements: python3 + pytorch
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

import torchvision as tv
import torchvision.transforms as transforms
from sklearn.metrics import confusion_matrix

import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import math
import os

train_batch_size=64
test_batch_size=1000

class Net(nn.Module):
    def __init__(self):
        super(Net,self).__init__()
        self.conv1=nn.Conv2d(1,6,5,stride=1,padding=2)
        self.conv2=nn.Conv2d(6,16,5,stride=1,padding=0)
        self.fc1=nn.Linear(16*5*5,120)
        self.fc2=nn.Linear(120,84)
        self.fc3=nn.Linear(84,10)
    def forward(self,x):
        x=F.max_pool2d(F.relu(self.conv1(x)),2)
        x=F.max_pool2d(F.relu(self.conv2(x)),2)
        x=x.view(-1,self.num_flat_features(x))
        x=F.relu(self.fc1(x))
        x=F.relu(self.fc2(x))
        x=self.fc3(x)
        return x
    def num_flat_features(self,x):
        #x is a 4D tensor
        x_size=x.size()[1:]
        num=1
        for n in x_size:
            num*=n
        return num

def load_data():
    train_transform=transforms.Compose(
        [transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))])
    test_transform=transforms.Compose(
        [transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))])
    train_set=tv.datasets.MNIST(
        root='./data',
        train=True,
        download=True,
        transform=train_transform
        )
    train_loader=torch.utils.data.DataLoader(
        train_set,
        batch_size=train_batch_size,
        shuffle=True,
        num_workers=2)
    test_set=tv.datasets.MNIST(
        root='./data',
        train=False,
        download=True,
        transform=test_transform
        )
    test_loader=torch.utils.data.DataLoader(
        test_set,
        batch_size=test_batch_size,
        shuffle=False,
        num_workers=2)
    print("data loaded successfully...")
    return train_loader,test_loader

def accuracy(model,x,neg=False):
    with torch.no_grad():
        correct=0
        total=0
        class_correct = list(0. for i in range(10))
        class_total = list(0. for i in range(10))
        for data in x:
            images,labels=data
            if neg:
            	images=-images
            images,labels=images, labels
            outputs=model(images)
            _,predicted=torch.max(outputs.data,1)
            total+=labels.size(0)
            correct+=(predicted==labels).sum().item()
            
        return (100 * correct / total)

def train(train_loader, test_loader, model, criterion, optimizer,epoch):
    model.train()
    running_loss=0
    for i,data in enumerate(train_loader,0):
        inputs,labels=data
        inputs,labels=inputs, labels
        optimizer.zero_grad()
        outputs=model(inputs)
        loss=criterion(outputs,labels)
        loss.backward()
        optimizer.step()

        # print statistics
        running_loss+=loss.item()
        if i%200==199:
            print("[epoch %d, iter %5d] loss: %.3f"%(epoch+1,i+1,running_loss/200))
            running_loss=0.0
    train_acc=accuracy(model,train_loader)
    test_acc=accuracy(model,test_loader)
    print("epoch %d: train_acc %.3f, test_acc %.3f"%(epoch+1,train_acc,test_acc))
    return train_acc,test_acc

def display_confusion_matrix(model, dataloader, neg=False):
    with torch.no_grad():
        all_predicted = []
        all_labels = []
        for data in dataloader:
            images,labels=data
            if neg:
                images = -images
            outputs = model(images)
            _, predicted = torch.max(outputs, 1)
            all_predicted.append(predicted.numpy()) 
            all_labels.append(labels.numpy())

    all_predicted = np.concatenate(all_predicted)
    all_labels = np.concatenate(all_labels)
    cm = confusion_matrix(all_labels, all_predicted)
    plt.figure(figsize=(10,8))
    sns.heatmap(cm, annot=True, fmt="d", cmap="Blues", xticklabels=range(10), yticklabels=range(10))
    plt.xlabel("Predicted labels")
    plt.ylabel("True labels")
    plt.title("Confusion Matrix")
    plt.show()

def findPicture(model, dataloader, o1, f1, o2, f2, o3, f3,neg=False):
    p1 = False
    p2 = False
    p3 = False
    with torch.no_grad():
        
        for data in dataloader:
            images,labels=data
            if neg:
                images = -images
            outputs = model(images)
            _, predicted = torch.max(outputs, 1)
            all_predicted = []
            all_labels = []
            all_predicted.append(predicted.numpy()) 
            all_labels.append(labels.numpy())

            all_predicted = np.concatenate(all_predicted)
            all_labels = np.concatenate(all_labels)
            for i in range(len(all_labels)):
                if(not p1 and all_labels[i] == o1 and all_predicted[i] == f1):
                    image = images[i]
                    image = image.squeeze().numpy()

                    plt.imshow(image)
                    plt.title(f'{o1} - {f1}')
                    plt.show()
                    p1 = True

                if(not p2 and all_labels[i] == o2 and all_predicted[i] == f2):
                    image = images[i]
                    image = image.squeeze().numpy()

                    plt.imshow(image)
                    plt.title(f'{o2} - {f2}')
                    plt.show()
                    p2 = True
                
                if(not p3 and all_labels[i] == o3 and all_predicted[i] == f3):
                    image = images[i]
                    image = image.squeeze().numpy()

                    plt.imshow(image)
                    plt.title(f'{o3} - {f3}')
                    plt.show()
                    p3 = True

            if(p1 and p2 and p3):
                break    
                

def display(train_acc1,test_acc1):

    fig,ax=plt.subplots()
    ax.plot(range(1,len(train_acc1)+1),train_acc1,label='trainAcc lr=0.001 dc=0.00001')
    ax.plot(range(1,len(test_acc1)+1),test_acc1,label='test_acc lr=0.001 dc=0.00001')
    ax.legend(loc='lower right')
    plt.show()

if __name__ == '__main__':
    # input MNIST
    train_loader,test_loader=load_data()
    # new model
    net1=Net()

    # training
    learning_rate1=0.001

    momentum=0.9
    max_epoch=4
    weight_decay1=0.00001

    criterion=nn.CrossEntropyLoss()
    optimizer_AdW1=optim.AdamW(net1.parameters(),lr=learning_rate1,weight_decay=weight_decay1)
    
    train_acc1=[]
    test_acc1=[]

    for epoch in range(max_epoch):
        train_acc_t1,test_acc_t1=train(train_loader, test_loader, net1, criterion, optimizer_AdW1,epoch)
        train_acc1.append(train_acc_t1)
        test_acc1.append(test_acc_t1)


    display(train_acc1,test_acc1)
    # display_confusion_matrix(net1, test_loader, neg=False)
    findPicture(net1, test_loader, 7, 2, 9, 5, 4, 9)

    