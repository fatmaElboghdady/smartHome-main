import cv2
import numpy as np
import face_recognition
import os

from firebase_admin import credentials,initialize_app,db
# import json
cred=credentials.Certificate(r"C:\Users\ha458\Desktop\project\Face_recognition\smarthome.json")
app =initialize_app(cred,{
    'databaseURL':'https://smarthomeapp-3a9d9-default-rtdb.firebaseio.com/'
})


path=r'C:\Users\ha458\Desktop\project\Face_recognition\person'

# to acess the images
images =[]
classNames=[]
personList=os.listdir(path)

# #to test the correct acess
# print(personList)

#to read the image
for c1 in personList:
    curPersonn=cv2.imread(f'{path}/{c1}')
    images.append(curPersonn)
    #take the name only without extention
    classNames.append(os.path.splitext(c1)[0])

# #to test take the name of image without the extention
# print(classNames)

#to make encode the img in path
def findEncodeings(image):
    encodeList=[]
    for img in images:
        img =cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
        encode=face_recognition.face_encodings(img)[0]
        encodeList.append(encode)
    return encodeList

# to get information of face and store it in variable
encodeListKnown=findEncodeings(images)
# print(encodeListKnown)
print("encoding complete")
    

#to know camera
# may be insert vide except camera by insert in partheness the path of video
cap=cv2.VideoCapture(0)


while True:
    _,img=cap.read()

    # reduece the img
    imgs=cv2.resize(img,(0,0),None,0.25,0.25)
    imgs =cv2.cvtColor(imgs,cv2.COLOR_BGR2RGB)

    #encode the face
    faceCurentFrame=face_recognition.face_locations(imgs)
    encodeCurrentFrame=face_recognition.face_encodings(imgs,faceCurentFrame)
    ref = db.reference("/Img/")
    check=0

    for encodeface,faceLoc in zip(encodeCurrentFrame,faceCurentFrame):
        #compare with the video and the data 
        matches=face_recognition.compare_faces(encodeListKnown,encodeface)
        #gave the persent of similer of vide and data
        faceDis=face_recognition.face_distance(encodeListKnown,encodeface)
        matchIndex=np.argmin(faceDis)
        
        if matches[matchIndex]:
            name=classNames[matchIndex].upper()
            # print( matches[matchIndex])
            x="Img open"
            check=0
            ref.set(x)
            
            print(name)
            y1,x2,y2,x1=faceLoc
            y1,x2,y2,x1=y1*4,x2*4,y2*4,x1*4
            cv2.rectangle(img,(x1,y1),(x2,y2),(0,0,255),2)
            cv2.rectangle(img,(x1,y2-35),(x2,y2),(0,0,255),cv2.FILLED)
            cv2.putText(img,name,(x1+6,y2-6),cv2.FONT_HERSHEY_COMPLEX,1,(255,255,255),2)
            break
    else:
        print("hagerjsdljdjo")
        if check==0:
            x="Img close"
            ref.set(x)
            ++check

    # for encodeface,faceLoc  in  zip(encodeCurrentFrame,faceCurentFrame):
    #     

    cv2.imshow("face Recogntion ",img)
    #if 1 then video but 0 then image
    if cv2.waitKey(5) & 0xFF ==27:
            break
    






