import pandas as pd
import MySQLdb

dataFrame = pd.read_csv("20061101.txt",delim_whitespace=True,names=["Duration","ServiceType",
"SrcBytes","DstBytes","Count","SrvRate","SerrorRate","SrvSerrorRate",
"DstHostCount","DstHostSrvCount","DstHostSameSrcPortRate","DstHostSerrorRate",
"DstHostSrvSerrorRate","Flag","IdsDection","MalwareDetection","AshulaDetection","Label",
"SrcIpAddr","SrcPortNo","DstIpAddr","DstPortNo","StartTime","TProtocol"])

#print(dataFrame[~dataFrame.ServiceType.isin(["http","smtp","dns","other","ftp","ftp-data","ssl","ssh","smtp,ssl"])]["ServiceType"])
#print(dataFrame[~dataFrame.TProtocol.isin(["tcp","udp","icmp"])]["TProtocol"])
normalData = dataFrame[dataFrame.Label == 1]
attackData = dataFrame[dataFrame.Label.isin([-1,-2])]
db = MySQLdb.connect("localhost","testuser","password","testdb")
if(normalData.shape[0]>0):
	normalData.to_sql(name="NormalData", con=db, if_exists='replace', flavor='mysql')
if(attackData.shape[0]>0):
	attackData.to_sql(name="AttackData", con=db, if_exists='replace', flavor='mysql')

#print(attackData.shape)
#print(normalData.shape)
#print(dataFrame.shape)

if (attackData.shape[0]>0):
    #Get TCP attack data 
    tcpAttackData = attackData[attackData.TProtocol == "tcp"]
    if(tcpAttackData.shape[0]>0):
	tcpAttackData.to_sql(name="TCPAttackData", con=db, if_exist='replace', flavor='mysql')
    if tcpAttackData.shape[0]>0:
        httpAttackData = tcpAttackData[tcpAttackData.ServiceType == "http"]
        if(httpAttackData.shape[0]>0):
		httpAttackData.to_sql(name="HTTPAttackData", con=db, if_exist='replace', flavor='mysql')
        smtpAttackData = tcpAttackData[tcpAttackData.ServiceType == "smtp"]
	if(smtpAttackData.shape[0]>0):
                smtpAttackData.to_sql(name="SMTPAttackData", con=db, if_exist='replace', flavor='mysql')
        sslAttackData = tcpAttackData[tcpAttackData.ServiceType == "ssl"]
	if(sslAttackData.shape[0]>0):
                sslAttackData.to_sql(name="SSLAttackData", con=db, if_exist='replace', flavor='mysql')
        sshAttackData = tcpAttackData[tcpAttackData.ServiceType == "ssh"]
 	if(sshAttackData.shape[0]>0):
                sshAttackData.to_sql(name="SSHAttackData", con=db, if_exist='replace', flavor='mysql')
    #Get UDP attack data
    udpAttackData = attackData[attackData.TProtocol == "udp"]
    if(udpAttackData.shape[0]>0):
                udpAttackData.to_sql(name="UDPAttackData", con=db, if_exist='replace', flavor='mysql')
    if udpAttackData.shape[0]>0:
        dnsAttackData = udpAttackData[udpAttackData.ServiceType == "dns"]
     	if(dnsAttackData.shape[0]>0):
                dnsAttackData.to_sql(name="DNSAttackData", con=db, if_exist='replace', flavor='mysql')
        
    #Get ICMP attack data
    icmpAttackData = attackData[attackData.TProtocol == "icmp"]
    if(icmpAttackData.shape[0]>0):
                icmpAttackData.to_sql(name="ICMPAttackData", con=db, if_exist='replace', flavor='mysql')
    
    

#normalData.set_index("SrcIpAddr",inplace = True)
#print(normalData)

