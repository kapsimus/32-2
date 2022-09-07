#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include "nlohmann/json.hpp"
#include "Film.h"

#define FILM_COUNT 5

struct Actor{
    std::string filmName;
    std::string personageName;
    std::string actorName;
};

void addRecord(Film& _newFilm, nlohmann::json& _filmList) {
    nlohmann::json record;
    record["studio"] = _newFilm.getStudio();
    record["date"] = _newFilm.getDate();
    int count = _newFilm.getScenaristsCount();
    if (count == 1) {
        record["script"] = _newFilm.getScenarist();
    } else if (count > 1) {
        record["script"] = _newFilm.getScenaristList();
    }
    count = _newFilm.getDirectorsCount();
    if (count == 1) {
        record["director"] = _newFilm.getDirector();
    } else if (count > 1) {
        record["director"] = _newFilm.getDirectorsList();
    }
    count = _newFilm.getProducersCount();
    if (count == 1) {
        record["producer"] = _newFilm.getProducer();
    } else if (count > 1) {
        record["producer"] = _newFilm.getProducersList();
    }
    std::vector<std::pair<std::string, std::string>> actors;
    _newFilm.getActors(actors);
    if (!actors.empty()) {
        nlohmann::json tempActors;
        for (int i = 0; i < actors.size(); i++) {
            tempActors[actors[i].first] = actors[i].second;
        }
        record["actors"] = tempActors;
    }
    _filmList[_newFilm.getName()] = record;
}

std::vector<Actor> findActor(nlohmann::json _filmList, const std::string& _name) {
    Actor actor;
    std::vector<Actor> result;
    nlohmann::json::iterator it, it2, it3;
    for (it = _filmList.begin(); it != _filmList.end(); it++) {
        nlohmann::json film = it.value();
        for (it2 = film.begin(); it2 != film.end(); it2++) {
            if (it2.key() == "actors") {
                nlohmann::json actors = it2.value();
                for (it3 = actors.begin(); it3 != actors.end(); it3++) {
                    std::string strActor = it3.value();
                    if (strActor.find(_name) != std::string::npos) {
                        actor.personageName = it3.key();
                        actor.actorName = it3.value();
                        actor.filmName = it.key();
                        result.push_back(actor);
                    }
                }
            }
        }
        if (it->contains(_name)) {
            actor.actorName = it.value();
            actor.personageName = it.key();
            result.push_back(actor);
        }
    }
    return result;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    nlohmann::json record;
    nlohmann::json filmList;
    std::ofstream file("film_base.json");
    Film film[FILM_COUNT];

    film[0].setName("Тот самый Мюнхгаузен");
    film[0].setStudio("Мосфильм");
    film[0].setDate(1980);
    film[0].setScenarist("Григорий Горин");
    film[0].setProducer("Центральное телевидение СССР");
    film[0].setDirector("Марк Захаров");
    film[0].setActor("барон Карл Фридрих Иероним фон Мюнхгаузен", "Олег Янковский");
    film[0].setActor("Марта, возлюбленная барона", "Елена Коренева");
    film[0].setActor("баронесса Якобина фон Мюнхгаузен", "Инна Чурикова");
    film[0].setActor("Генрих Рамкопф, любовник баронессы", "Александр Абдулов");
    film[0].setActor("Феофил фон Мюнхгаузен, сын барона", "Леонид Ярмольник");
    film[0].setActor("Томас, слуга барона", "Юрий Катин-Ярцев");

    film[1].setName("Обыкновенное чудо");
    film[1].setStudio("Мосфильм");
    film[1].setDate(1978);
    film[1].setScenarist("Григорий Горин");
    film[1].setProducer("Первая программа ЦТ");
    film[0].setDirector("Марк Захаров");
    film[1].setActor("юноша-медведь", "Александр Абдулов");
    film[1].setActor("хозяин (Волшебник)", "Олег Янковский");
    film[1].setActor("король", "Евгений Леонов");
    film[1].setActor("принцесса", "Евгения Симонова");
    film[1].setActor("хозяйка, жена Волшебника", "Ирина Купченко");
    film[1].setActor("министр-администратор", "Андрей Миронов");

    film[2].setName("Бриллиантовая рука");
    film[2].setStudio("Мосфильм");
    film[2].setDate(1968);
    film[2].setScenarist("Леонид Гайдай");
    film[2].setScenarist("Яков Костюковский");
    film[2].setScenarist("Морис Слободской");
    film[2].setProducer("Госкино СССР");
    film[0].setDirector("Леонид Гайдай");
    film[2].setActor("Семён Семёнович Горбунков", "Юрий Никулин");
    film[2].setActor("Надежда Ивановна, жена Горбункова", "Нина Гребешкова");
    film[2].setActor("Лёлик (Механик)", "Анатолий Папанов");
    film[2].setActor("Геннадий Петрович Козодоев (Геша, Граф)", "Андрей Миронов");
    film[2].setActor("Михаил Иванович, милиционер", "Станислав Чекан");
    film[2].setActor("Варвара Сергеевна Плющ, управдом", "Нонна Мордюкова");

    film[3].setName("Кин-дза-дза!");
    film[3].setStudio("Мосфильм");
    film[3].setDate(1986);
    film[3].setScenarist("Георгий Данелия");
    film[3].setScenarist("Резо Габриадзе");
    film[3].setProducer("Николай Михайлович Гаро");
    film[3].setDirector("Георгий Данелия");
    film[3].setActor("Владимир Николаевич Машков («дядя Вова»), прораб", "Станислав Любшин");
    film[3].setActor("Гедеван Александрович Алексидзе («Скрипач»), студент", "Леван Габриадзе");
    film[3].setActor("Уэф, чатланин", "Евгений Леонов");
    film[3].setActor("Би, пацак", "Юрий Яковлев");

    film[4].setName("Афоня");
    film[4].setStudio("Мосфильм");
    film[4].setDate(1975);
    film[4].setScenarist("Александр Бородянский");
    film[4].setProducer("Госкино СССР");
    film[4].setDirector("Георгий Данелия");
    film[4].setActor("Афанасий Николаевич Борщов (Афоня), слесарь-сантехник", "Леонид Куравлёв");
    film[4].setActor("Катя Снегирёва, медсестра", "Евгения Симонова");
    film[4].setActor("Коля, штукатур", "Евгений Леонов");
    film[4].setActor("Егоза, тракторист, друг детства Афони", "Савелий Крамаров");
    film[4].setActor("Федулов (Федул), грузчик, собутыльник Афони", "Борислав Брондуков");

    for (int i = 0; i < FILM_COUNT; i++){
        addRecord(film[i], filmList);
    }
    //std::cout << filmList << std::endl;
    file << filmList;
    file.close();

    std::ifstream jsonFile("film_base.json");
    if (jsonFile.is_open()) {
        nlohmann::json inputList;
        jsonFile >> inputList;
        std::vector<Actor> searchResult = findActor(inputList, "Юрий");
        if (!searchResult.empty()) {
            for (Actor actor: searchResult) {
                std::cout << "Film name: " << actor.filmName << std::endl;
                std::cout << "Personage: " << actor.personageName << std::endl;
                std::cout << "Actor: " << actor.actorName << std::endl << std::endl;
            }
        }
        jsonFile.close();
    }
    return 0;
}
