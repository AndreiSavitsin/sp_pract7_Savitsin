#include <iostream>
#include <fstream>
#include <string>

#define DEBUG

class Record {
    int number;
    std::string namePatient;
    std::string nameDoctor;
    std::string date;
    std::string time;
    std::string status;

public:

    Record(int _num, std::string _namePatient, std::string _nameDoctor, std::string _date, std::string _time, std::string _status)
    {
        if (_namePatient != "" && _nameDoctor != "" && _date != "" && _time != "" && _status != "")
        {
            number = _num;
            namePatient = _namePatient;
            nameDoctor = _nameDoctor;
            date = _date;
            time = _time;
            status = _status;
        }
        else
        {
            throw std::invalid_argument("Поля должны быть заполнены");
        }
    }

    Record() {}

    int GetNum()
    {
        return number;
    }

    std::string GetNamePatient()
    {
        return namePatient;
    }

    std::string GetNameDoctor()
    {
        return nameDoctor;
    }
    
    std::string GetDate()
    {
        return date;
    }

    std::string GetTime()
    {
        return time;
    }

    std::string GetStatus()
    {
        return status;
    }

    void SetStatus(std::string _status)
    {
        status = _status;
    }

    void PrintInfo()
    {
        std::cout << "\nНомер записи: " << number << std::endl;
        std::cout << "Имя пациента: " << namePatient << std::endl;
        std::cout << "Имя врача: " << nameDoctor << std::endl;
        std::cout << "Дата записи: " << date << std::endl;
        std::cout << "Время записи: " << time << std::endl;
        std::cout << "Статус приёма: " << status << std::endl;
    } 
};

class ArrayRecords {

    static Record records[10];

    static int count;

public:

    static void LoadFromFile()
    {
        try
        {
            std::ifstream file("file.txt");

            if (file.is_open())
            {
                std::string line;

                count = 0;

                while (std::getline(file, line))
                {
                    std::string parts[6];

                        if (line.find(";") == std::string::npos)
                        {
                            throw std::invalid_argument("Файл заполнен некорректно");
                        }

                        for (int i = 0; i < 5; i++)
                        {
                            int separator = line.find(";");
                            if (separator == std::string::npos)
                            {
                                throw std::invalid_argument("Недостаточно данных в строке");
                            }
                            parts[i] = line.substr(0, separator);
                            line.erase(0, separator + 1);
                        }

                        std::string date;
                        date = parts[3];
                        int separator = date.find(".");
                        int day = std::stoi(date.substr(0, separator));
                        date.erase(0, separator + 1);
                        if (day < 1 || day > 31)
                        {
                            throw std::out_of_range("День должен быть от 1 до 31");
                        }

                        separator = date.find(".");
                        int month = std::stoi(date.substr(0, separator));
                        date.erase(0, separator + 1);
                        if (month < 1 || month > 12)
                        {
                            throw std::out_of_range("Месяц должен быть от 1 до 12");
                        }

                        int year = std::stoi(date);

                        std::string time;
                        time = parts[4];
                        separator = time.find(".");

                        int hour = std::stoi(time.substr(0, separator));
                        time.erase(0, separator + 1);
                        if (hour < 0 || hour > 23)
                        {
                            throw std::out_of_range("Час должен быть от 0 до 23");
                        }

                        int min = std::stoi(time);
                        if (min < 0 || min > 59)
                        {
                            throw std::out_of_range("Минута должна быть от 0 до 59");
                        }

                        parts[5] = line;

                        int number = std::stoi(parts[0]);

                        Record record(number, parts[1], parts[2], parts[3], parts[4], parts[5]);

                        if (count >= 10)
                        {
                            std::cout << "Массив заполнен.";
                            break;
                        }

                        records[count] = record;

                        count++;

#ifdef DEBUG
                        std::cout << "\n[DEBUG] Количество загруженных записей: " << count << std::endl;
                        record.PrintInfo();
#endif // DEBUG

                }

                file.close();
            }
            else
            {
                std::cout << "Файл не существует или не может открыться\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Ошибка: " << e.what();
        }
    }

    static void SearchRecordByNumber(int num)
    {        
        std::cout << "\nЗаписи по номеру: " << num << std::endl;
        int count2 = 0;

        for (int i = 0; i < count; i++)
        {
            if (records[i].GetNum() == num)
            {
                records[i].PrintInfo();
                count2++;
            }
        }

        if (count2 == 0)
        {
            std::cout << "Записей не найдено\n";
        }
    }

    static void SearchRecordByPatient(std::string namePatient)
    {
        std::cout << "\nЗаписи по имени пациента: " << namePatient << std::endl;
        int count2 = 0;

        for (int i = 0; i < count; i++)
        {
            if (records[i].GetNamePatient() == namePatient)
            {
                records[i].PrintInfo();
                count2++;
            }
        }

        if (count2 == 0)
        {
            std::cout << "Записей не найдено\n";
        }
    }

    static void CancelRecord(int num)
    {
        int count2 = 0;
        int tempI = 0;
        for (int i = 0; i < count; i++)
        {
            if (records[i].GetNum() == num)
            {
                records[i].SetStatus("Cancelled");
                tempI = i;
                count2++;
            }
        }

        if (count2 == 0)
        {
            std::cout << "Записей не найдено\n";
        }
        else
        {
            std::cout << "Запись отменена\n";
            records[tempI].PrintInfo();
        }
    }

    static void SearchRecordByDoctor(std::string nameDoctor)
    {
        std::cout << "\nЗаписи по имени доктора: " << nameDoctor << std::endl;
        int count2 = 0;

        for (int i = 0; i < count; i++)
        {
            if (records[i].GetNameDoctor() == nameDoctor)
            {
                records[i].PrintInfo();
                count2++;
            }
        }

        if (count2 == 0)
        {
            std::cout << "Записей не найдено\n";
        }
    }

    static void StatusChange(int day, int month, int year)
    {
        std::string date;

        for (int i = 0; i < count; i++)
        {
            date = records[i].GetDate();

            int separator = date.find(".");
            int day2 = std::stoi(date.substr(0, separator));
            date.erase(0, separator + 1);

            separator = date.find(".");
            int month2 = std::stoi(date.substr(0, separator));
            date.erase(0, separator + 1);

            int year2 = std::stoi(date);

            if (year2 < year)
            {
                records[i].SetStatus("Completed");
            }
            else if (year2 == year)
            {
                if (month2 < month)
                {
                    records[i].SetStatus("Completed");
                }
                else
                {
                    if (month2 == month)
                    {
                        if (day2 < day)
                        {
                            records[i].SetStatus("Completed");
                        }
                    }
                }
            }
        }
    }

    static void SaveToFile()
    {
        std::ofstream file("file.txt");

        for (int i = 0; i < count; i++)
        {
            file << records[i].GetNum() << ";" << records[i].GetNamePatient() << ";"
                << records[i].GetNameDoctor() << ";" << records[i].GetDate() << ";"
                << records[i].GetTime() << ";" << records[i].GetStatus() << "\n";
        }

        file.close();
    }
};

Record ArrayRecords::records[10];
int ArrayRecords::count = 0;

int main()
{
    setlocale(LC_ALL, "Russian");

#ifdef DEBUG

    try
    {
        ArrayRecords::LoadFromFile();

        //1
        std::cout << "\nВведите номер записи для поиска: ";
        int num;
        std::cin >> num;
        ArrayRecords::SearchRecordByNumber(num);

        //2
        std::cout << "\nВведите имя пациента для поиска: ";
        std::string namePatient;
        std::cin >> namePatient;
        ArrayRecords::SearchRecordByPatient(namePatient);

        //3
        std::cout << "\nВведите имя доктора для поиска: ";
        std::string nameDoctor;
        std::cin >> nameDoctor;
        ArrayRecords::SearchRecordByDoctor(nameDoctor);

        //4
        std::cout << "\nВведите номер записи для отмены: ";
        std::cin >> num;
        ArrayRecords::CancelRecord(num);

        //5
        std::cout << "Введите сегодняшнюю дату (пример записи: 01.01.2026): ";
        std::string date;
        std::cin >> date;
        if (date.find(".") == std::string::npos)
        {
            throw std::invalid_argument("Неверный формат записи. Пример записи: 01.01.2026\n");            
        }
        else
        {
            int separator = date.find(".");
            int day = std::stoi(date.substr(0, separator));
            date.erase(0, separator + 1);
            if (day < 1 || day > 31)
            {
                throw std::out_of_range("День должен быть от 1 до 31");
            }

            separator = date.find(".");
            int month = std::stoi(date.substr(0, separator));
            date.erase(0, separator + 1);
            if (month < 1 || month > 12)
            {
                throw std::out_of_range("Месяц должен быть от 1 до 12");
            }

            int year = std::stoi(date);

            ArrayRecords::StatusChange(day, month, year);
        }

        //6
        ArrayRecords::SaveToFile();

    }
    catch (const std::exception& e)
    {
        std::cout << "Ошибка: " << e.what();
    }

#endif //DEBUG
}