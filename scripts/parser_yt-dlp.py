import yt_dlp
import json
import argparse


def get_video_info(url):
    ydl_opts = {
        'quiet': True,
        'format': 'best',
    }

    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        try:
            info = ydl.extract_info(url, download=False)
            video_url = info['url']
            video_title = info.get('title', 'No title found')
            channel = info.get('uploader', 'No channel found')
            seconds = info.get('duration', 'No duration found')
            return video_url, video_title, channel, seconds
        except yt_dlp.utils.DownloadError as e:
            print(f"Ошибка загрузки: {e}")
        except yt_dlp.networking.exceptions.TransportError as e:
            print(f"Ошибка сети: {e}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Пример передачи параметров.")
    parser.add_argument('--url', type=str, help='Ссылка на трек', required=True)
    args = parser.parse_args()
    url, title, channel, seconds = get_video_info(args.url)
    if url:
        print(f"Видео URL: {url}")
        print(f"Название видео: {title}")
        print(f"Канал: {channel}")
        print(f"Канал: {seconds}")

    data = {'title': title, 'author': channel, 'url': url, 'duration': seconds}

    with open('data.json', 'w') as file:
        json.dump(data, file)

