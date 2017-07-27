import urllib2
from bs4 import BeautifulSoup
from datetime import datetime

class system_page:

    username = ''
    password = ''
    ip_tag = 'itemdesc_text'

    def __init__(self):
        self.username = 'adm'
        self.password = 'adm'
        #if admin id is not <adm>,  Use <set_info> method

    def set_info(self,name,passwd):
        self.username = name
        self.password = passwd

    def open_page(self):
        url = 'http://192.168.0.1/cgi-bin/timepro.cgi?tmenu=netconf&smenu=laninfo'
        #default name value
        username = self.username
        password = self.password
        p = urllib2.HTTPPasswordMgrWithDefaultRealm()

        p.add_password(None, url, username, password)

        handler = urllib2.HTTPBasicAuthHandler(p)
        opener = urllib2.build_opener(handler)
        urllib2.install_opener(opener)

        page = urllib2.urlopen(url).read()
       # print page
        return page

    def run_soup(self):
        obj = self.open_page()
        soup = BeautifulSoup(obj, "lxml")
        soup_f = soup.find_all("table",{"class":"menu_content_list_table"})[1]
        temp = [] #span",{"class":self.ip_tag}
        for connected_ip_tag in soup_f.find_all("tr")[1:]:
            data_string = connected_ip_tag.find_all("span")[1].string
            temp.append(data_string)
        return temp

class controll_data:
    lab_member = {} #dictionary { name : ip_address }

    def __init__(self):
        self.load_data()

    def save_data(self,name,device_name):#member.txt file save member name. save format [name Device_name]
        with open('./member.txt', 'a') as f:
            data = device_name + '.' + name
            f.write(data)

    def load_data(self):
        f = open('./member.txt','r')
        lines = f.readlines()
        for line in lines:
            self.lab_member[line.split('.')[0]] = line.split('.')[1].split('\n')[0]

    def save_log(self,device,msg):
        now = datetime.now()
        try:
            file_name = self.lab_member[device] + '.txt'
        except:
            file_name = 'Unknown.txt'

        with open(file_name,'a') as f:
            f.write(now.ctime() +"  " +  msg +'\n')
