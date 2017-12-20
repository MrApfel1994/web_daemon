
import os
import urllib2
import zipfile

def download_file(id, file_name):
    print 'downloading', file_name
    
    url = "https://docs.google.com/uc?export=download&id=" + id

    u = urllib2.urlopen(url)
    
    
    confirm_token = None
    
    i = u.info()
    if i.type == 'text/html':
        buffer = u.read()
        i1 = buffer.find('confirm=')
        if i1 != -1:
            i2 = buffer.find('&', i1)
            if i2 != -1:
                confirm_token = buffer[i1 + 8 : i2]
        print buffer
        f = open(file_name, 'wb')
        f.write(buffer)
    
    if confirm_token != None:
        url = "https://docs.google.com/uc?export=download&confirm=" + confirm_token + "&id=" + id
        print url
        u = urllib2.urlopen(url)
        #print u
    
    #return
    
    f = open(file_name, 'wb')

    file_size_dl = 0
    block_sz = 32768
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break
        file_size_dl += len(buffer)
        f.write(buffer)
        #print file_size_dl, 'bytes written'
    f.close()

def unzip_file(file_name, unzip_dest):
    print 'extracting', file_name, 'to', unzip_dest
    
    zip_ref = zipfile.ZipFile(file_name, 'r')
    zip_ref.extractall(unzip_dest)
    zip_ref.close()

if os.name == 'nt':
    download_file('1rkvUF6rrB3bWyi47-2hcMYCN_8tmw5Zb', 'libs.zip')
else:
    download_file('1vjiU9jbtFgJBwWypQ_JPrF2lY3GUqb_s', 'libs.zip')
unzip_file('libs.zip', 'src/')
os.remove('libs.zip')
