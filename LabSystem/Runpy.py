import CheckIp
from time import sleep
from threading import Thread

def check_change(old, new): # Check member state
    k = CheckIp.controll_data()
    for member in old:
        if new.count(member):
            continue
        message = "Device Name <" + member + "> out"
        print message
        k.save_log(member,message)

    for member in new:
        if old.count(member):
            continue
        message = "Device Name <" + member +"> in"
        print message
        k.save_log(member,message)

if __name__=='__main__':
    member_list = CheckIp.controll_data().lab_member
    Obj = CheckIp.system_page()
    old = []
    for i in range(10):
        sleep(3)
        text = Obj.run_soup()
        temp_list = []
        for ip_add in text: #get connected ip_name
            temp = ip_add.split('.')[3].split('(')
            try:
                temp_list.append(temp[1][:-1])
            except:
                temp_list.append('Unknown')
        check_change(old,temp_list)
        old = temp_list

