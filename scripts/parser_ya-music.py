import os
from yandex_music import Client
from dotenv import load_dotenv
import json
import argparse

load_dotenv()

client = Client(os.getenv('TOKEN_YA_M')).init()

def get_audio_id(track_id, album_id, link):
    result = f"{track_id}:{album_id}"
    track = client.tracks(result)[0]

    if (link):
        url = track.get_specific_download_info('mp3', 192).get_direct_link()
    else:
        url = "none"
    title = track.title
    author = ", ".join([author.name for author in track.artists])
    duration = track.duration_ms/1000
    cover = track.cover_uri

    data = {'track_id': track_id, 'album_id': album_id, 'title': title, 'author': author, 'url': url, 'duration': duration, 'cover': cover}

    return data

def get_audio_url(url):
    parts = url.split('/')
    album_id = parts[-3]
    track_id = parts[-1]

    return get_audio_id(track_id, album_id, False)


def send_search_request_and_print_result(query):
    search_result = client.search(query)
    results = search_result.tracks.results

    search_results = {
        i: {
            'title': result.title,
            'author': ", ".join(author.name for author in result.artists),
            'url': result.get_specific_download_info('mp3', 192).get_direct_link(),
            'cover': result.cover_uri
        }
        for i, result in enumerate(results)
    }

    return search_results


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('query', type=str, help='Ссылка/Поисковый запрос')
    parser.add_argument('-t', "--type", choices=("url", "search", "id", "link"), help="url - поиск по ссылке, search - поиск по запросу, id - поиск по id трека и альбома, link - получение ссылки на аудио по id")
    #Пример команды python parser_ya-music.py -t url https://music.yandex.ru/album/17648600/track/89760037

    args = parser.parse_args()

    if args.type == "url":
        data = get_audio_url(args.query)
    elif args.type == "search":
        data = send_search_request_and_print_result(args.query)
    elif args.type == "id":
        tr, al = args.query.split(':')
        data = get_audio_id(tr, al, False)
    elif args.type == "link":
        tr, al = args.query.split(':')
        data = get_audio_id(tr, al, True)

    with open('data.json', 'w') as file:
        json.dump(data, file)
