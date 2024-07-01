import moviepy.editor
import moviepy.video
import moviepy.video.VideoClip
import requests
from urllib.parse import urlencode
import re
import json
import time
import os
import tqdm
import moviepy
from concurrent.futures import ThreadPoolExecutor

class Downloader:
    def __init__(self,url,name='bilibili',path='') -> None:
        self.headers={ 
        "User-Agent":'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36',
        'referer':'https://www.bilibili.com'
        }
        self.initial_url='https://www.bilibili.com/{}'
        self.base_url=url
        self.response=''
        self.text=''
        self.name=name
        self.finalpath=''
        self.video_path=''
        self.audio_path=''
        self.title=''
        if(path==''):
            self.path="D:\VideoLib"
        else:
            self.path=path

    def AskJson(self):
        response=requests.get(url=self.base_url,headers=self.headers)
        self.text=response.text
        #print(self.text)
        playinfo=re.findall(r'<script>window.__playinfo__=(.*?)</script>',response.text)[0]
        playinfo_json=json.loads(playinfo)
        return playinfo_json,response
    
    def GetUrl(self):
        Json,response=self.AskJson()
        self.response=response
        # 视频与音频文件地址
        video_url = Json['data']['dash']['video'][0]['base_url']
        audio_url = Json['data']['dash']['audio'][0]['base_url']
        return video_url,audio_url
    
    def GetTitle(self):
        return re.findall(r'<title data-vue-meta="true">(.*?)_哔哩哔哩_bilibili</title>',self.text)[0]
    
    def Download(self,url,type):
        #tqdm.tqdm(resp.iter_content(chunk_size=chunk_size),total=int(file_size * 1024 * 1024 // chunk_size),unit='byte',unit_scale=True,desc='下载进度')
        filer_path=os.path.join(self.path,self.name)
        print(filer_path)
        title=self.GetTitle()
        self.title=title
        resp=requests.get(url=url,headers=self.headers)
        print(f'downloading:{type} file, status:{resp.status_code}, file_name:{title}')
        chunk_size=1024
        done_size=0
        file_size=int(resp.headers['content-length'])/1024/1024
        print(f'file_size:{file_size:0.2f}MB')
        start_time=time.time()
        if not os.path.exists(filer_path):
            os.mkdir(filer_path)
        self.finalpath=filer_path
        subfiler_path=os.path.join(filer_path,type)
        if not os.path.exists(subfiler_path):
            os.mkdir(subfiler_path)
        file_path=os.path.join(subfiler_path,title+type)
        with open(file_path,mode='wb') as f:
            for chunk in resp.iter_content(chunk_size=chunk_size):
                f.write(chunk)
                done_size+=len(chunk)/1024/1024
                print('\r'+f'下载进度：{done_size:0.2f}MB/{file_size:0.2f}MB, processing:{100*done_size/file_size:0.2f}%',end='',flush=True)
        end_time=time.time()
        delta_time=end_time-start_time
        print(f'\nDownloading complete\nCost_Time:{delta_time:0.2f}s\nspeed:{file_size/delta_time:0.2f}MB/s\n')
        return file_path
    
    def Combine(self,path):
        file_path=os.path.join(path,self.title+'.mp4')
        audio=moviepy.editor.AudioFileClip(self.audio_path)
        video=moviepy.editor.VideoFileClip(self.video_path).set_audio(audio)
        video.write_videofile(f'{file_path}')


    def Main_downloader(self):
        video_url,audio_url=self.GetUrl()
        print(f'status:{self.response.status_code}')
        self.video_path=self.Download(url=video_url,type='.mp4')
        self.audio_path=self.Download(url=audio_url,type='.mp3')
        path=os.path.join(self.finalpath,'video')
        if not os.path.exists(path=path):
            os.mkdir(path)
        self.Combine(path)
        return 'done'
    
    def Multi_Downloader(self,url):
        bv_list=self.GetbvList(url)
        task=[]
        with ThreadPoolExecutor(max_workers=5) as pool:
            for bv in bv_list:
                url='https://www.bilibili.com/{}'.format(bv)
                sub_downloader=Downloader(url=url,name='bilibili')
                task.append(pool.submit(sub_downloader.Main_downloader))
            result=[i.result() for i in task]
            print(f"result:{result}")       

    def GetbvList(self,url):
        api_url='https://api.bilibili.com/x/polymer/web-space/seasons_archives_list?'
        bv_list=[]
        form={'mid': '',
            'season_id': '',
            'page_num':'',
            'page_size':'30'}
        #https://space.bilibili.com/553815787/channel/collectiondetail?sid=1867458
        mid,sid=re.findall(r'space\.bilibili\.com/(.*?)/channel/collectiondetail\?sid=(.*?)$',url)[0]
        print(f'mid={mid},sid={sid}')
        form['mid']=mid
        form['season_id']=sid
        for page_num in range(10):
            form['page_num']=page_num+1
            Page_Json=self.GetJson0(form=form,url=api_url)
            page_bv_list=Page_Json['data']['archives']
            for dic in page_bv_list:
                bv_list.append(dic['bvid'])
            if(len(page_bv_list)<30):
                return bv_list


    def GetJson0(self,form:dict,url:str):
        real_url=url+urlencode(form)
        print(real_url)
        response=requests.get(url=real_url,headers=self.headers)
        print(response.status_code)
        if response.status_code==200:
            try:
                Json=response.json()
                print(Json)
            except json.JSONDecodeError:
                print('unable to convert to json')
        return Json

if __name__ == '__main__':
    base_url='https://www.bilibili.com/{}'
    print('1.single downloader\n2.multi-downloader\ninput the number to choose the mode')
    flag=int(input('mode:'))
    if flag==1:
        url=input('input the BV code:')
        downloader=Downloader(url=base_url.format(url))
        downloader.Main_downloader()
    else:
        url=input('input the url of the page:')
        downloader=Downloader(url='')
        downloader.Multi_Downloader(url=url)
