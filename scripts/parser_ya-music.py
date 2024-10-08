from yandex_music import Client
import json
import argparse

def get_audio_info(url):
    client = Client().init()

    parts = url.split('/')
    album_id = parts[-3]
    track_id = parts[-1]

    result = f"{track_id}:{album_id}"
    track = client.tracks(result)[0]

    audio_url = track.get_specific_download_info('mp3', 192).get_direct_link()
    audio_title = track.title
    audio_author = ", ".join([author.name for author in track.artists])
    audio_duration = track.duration_ms/1000

    return audio_url, audio_title, audio_author, audio_duration
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Пример передачи параметров.")
    parser.add_argument('--url', type=str, help='Ссылка на трек', required=True)
    args = parser.parse_args()
    url, title, author, duration = get_audio_info(args.url)
    print(url, title, author, duration)

    data = {'title': title, 'author': author, 'url': url, 'duration': duration}

    with open('data.json', 'w') as file:
        json.dump(data, file)
