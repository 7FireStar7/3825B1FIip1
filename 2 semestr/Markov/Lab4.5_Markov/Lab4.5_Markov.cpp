#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <locale>

using namespace std;

class Date{
public:
    chrono::year_month_day date;

    Date() = default;
    
    Date(int d, int m, int y) : date{chrono::year(y)/chrono::month(m)/chrono::day(d)} {};

    friend istream& operator>>(istream& is, Date& d){
        int day, month, year;
        char t;
        is >> day >> t >> month >> t >> year; 
        if (is){
            d.date = chrono::year(year)/chrono::month(month)/chrono::day(day);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Date& d){
        os << d.date.day() << '.' << static_cast<unsigned>(d.date.month()) << '.' << d.date.year();
        return os;
    }

};

class Song{
public:
    string title = "";
    string poet = "";
    string composer = "";
    string artist = "";
    string album = "";
    Date date;

    Song() = default;

    Song(const string& t, const string& p, const string& c,
         const string& a, const string& alb, const Date& d)
        : title(t), poet(p), composer(c), artist(a), album(alb), date(d) {}

    bool operator<(const Song& other) const {
        return title < other.title;
    }

};

bool comp(const Song& a, const Song& b){
    return a.title < b.title;
}

class Songbook {
private:
    vector<Song> songs;

    void sortSongs() {
        sort(songs.begin(), songs.end(), comp);
    }
public:
    Songbook() = default;

    // Добавление песен
    void addSong(const Song& song){
        songs.push_back(song);
        sortSongs();
    }

    void addSong(const string& title, const string& poet,
                 const string& composer, const string& artist,
                 const string& album, const Date& releaseDate) {
        songs.push_back(Song(title, poet, composer, artist, album, releaseDate));
        sortSongs();
    }

    vector<Song>::iterator getEndIter(){
        return songs.end();
    }

    // Поиск песни по названию и исполнителю, при негативном сценарии вернет итератор на конец вектора
    vector<Song>::iterator findSong(const string& title, const string& artist) {
        for (auto it = songs.begin(); it != songs.end(); it++) {
            if (it->title == title && it->artist == artist) {
                return it;    
            }   
        }
        return songs.end();
    }

    // Изменить данные выбранной песни
    bool editSong(const string& title, const string& artist, const Song& newData) {
        auto it = findSong(title, artist);
        if (it != songs.end()){
            string newTitle = newData.title;
            *it = newData;  
        
            if (newTitle != title) {
                sortSongs();
            }
            return true;
        };
        return false;        
    }    

    // Выдать все песни заданного поэта
    vector<Song> getSongsByPoet(const string& poet) const {
        vector<Song> result;
        for (const auto& i : songs){
            if (i.poet == poet){
                result.push_back(i);
            }
        }
        return result;
    }

    // Выдать все песни заданного композитора
    vector<Song> getSongsByComposer(const string& composer) const {
        vector<Song> result;
        for (const auto& i : songs){
            if (i.composer == composer){
                result.push_back(i);
            }
        }
        return result;
    }

    // Выдать все песни заданного исполнителя
    vector<Song> getSongsByArtist(const string& artist) const {
        vector<Song> result;
        for (const auto& i : songs){
            if (i.artist == artist){
                result.push_back(i);
            }
        }
        return result;
    }

    // Узнать текущее число песен
    size_t getSongCount() const {
        return songs.size();
    }

    // Удалить песню по названию и исполнителю
    bool removeSong(const std::string& title, const std::string& artist) {
        auto it = findSong(title, artist);
        if (it != songs.end()){
            songs.erase(it);
            return true;
        };
        return false;       
    }
  
    // Сохранить песенник в файл 
    bool saveToFile(const string& file) const {
        ofstream out(file);
        if (!out) return false;
        for (const auto& s : songs) {
            out << s.title << '|' << s.poet << '|' << s.composer << '|'
                << s.artist << '|' << s.album << '|' << s.date << '\n';
        }
        return true;
    }

    // Считать песенник из файла
    bool loadFromFile(const string& file) {
        ifstream in(file);
        if (!in) return false;
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            Song s;
            string dateStr;
            getline(ss, s.title, '|');
            getline(ss, s.poet, '|');
            getline(ss, s.composer, '|');
            getline(ss, s.artist, '|');
            getline(ss, s.album, '|');
            getline(ss, dateStr); 
            stringstream dateStream(dateStr);
            dateStream >> s.date;
            songs.push_back(s);
        }
        sortSongs();  
        return true;
    }
};

int main(){
    setlocale(LC_ALL, "Russian");  
    Songbook collection;
    ofstream out("ourt.txt");

    collection.loadFromFile("songbook.txt");
    
    collection.addSong("Песня 1", "Народные", "Народная", "Ансамбль", "", Date(1,2,1860));
    collection.addSong("Интересная Песня 2", "Поэт 2", "Композитор 2", "Исполнитель 2", "Альбом 2", Date(15,4,2014));
    collection.addSong("Невероятная песня 3", "Поэт 3", "Композитор 3", "Исполнитель 3", "Альбом 3", Date(9,9,1975));

    cout << "Текущее число песен: " << collection.getSongCount() << endl;

    string searchTitle = "Песня 1", searchArtist = "Ансамбль";
    auto it = collection.findSong(searchTitle, searchArtist);
    if (it != collection.getEndIter()) {
        cout << "Найдена песня: " << it->title << ", исполнитель: " << it->artist << endl;
    } else {
        cout << "Песня не найдена" << endl;
    }

    Song newData("Песня 1", "Новый поэт", "Народная", "Ансамбль", "Альбом", Date(1,2,1860));
    if (collection.editSong(searchTitle, searchArtist, newData)) {
        cout << "Песня изменена успешно" << endl;
        auto it2 = collection.findSong("Песня 1", "Ансамбль");
        if (it2 != collection.getEndIter()) {
            cout << "Новые данные: поэт: " << it2->poet << endl;
        }
    }

    string poetName = "Поэт 2";
    auto poetSongs = collection.getSongsByPoet(poetName);
    cout << "Песни поэта " << poetName << ":" << endl;
    for (const auto& s : poetSongs) {
        cout << "- " << s.title << endl;
    }

    string compName = "Композитор 3";
    auto compSongs = collection.getSongsByComposer(compName);
    cout << "Песни композитора " << compName << ":" << endl;
    for (const auto& s : compSongs) {
        cout << "- " << s.title << endl;
    }

    string artName = "Исполнитель 2";
    auto artSongs = collection.getSongsByArtist(artName);
    cout << "Песни исполнителя " << artName << ":" << endl;
    for (const auto& s : artSongs) {
        cout << "- " << s.title << endl;
    }

    collection.saveToFile("songbook.txt");

    return 0;
}