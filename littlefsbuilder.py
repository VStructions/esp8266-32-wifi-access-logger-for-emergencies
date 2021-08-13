Import("env")
import os

dataFolder = 'data'

if not dataFolder in os.listdir(os.getcwd()):
    os.mkdir(dataFolder)
    print("Empty \"data\" folder for empty filesystem creation ready")

print("Replace MKSPIFFSTOOL with mklittlefs.exe")
env.Replace (MKSPIFFSTOOL = "mklittlefs.exe")