#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

using std::cin;
using std::endl;
using std::cout;

enum Type {
    CAR, BOAT
};

struct Vehicle
{
    Vehicle* next;
    char model[20];
    Type type;
    union
    {
        struct
        {
            float area;
        } deck;
        struct
        {
            double pressure;
            int width;
        } wheels;
    };
};

Vehicle* head = NULL;
Vehicle* tail = NULL;
int size = 0;

void printAll()
{
    cout << "----------------\n";
    Vehicle* now = head;
    for (int i=0;i<size;i++)
    {
        cout << now->model << endl;
        if (now->type == BOAT)
        {
            cout << "Площадь палубы: " << now->deck.area << endl;
        }
        else
        {
            cout << "Давление в шинах: " << now->wheels.pressure << endl;
            cout << "Ширина шины: " << now->wheels.width << endl;
        }
        cout << "----------------\n";
        now = now->next;
    }
}

Vehicle* createBoat(const char model[], float area)
{
    Vehicle* newBoat = new Vehicle;
    strcpy(newBoat->model, model);
    newBoat->type = BOAT;
    newBoat->deck.area = area;
    return newBoat;
}

Vehicle* createCar(const char model[], double pressure, int width)
{
    Vehicle* newCar = new Vehicle;
    strcpy(newCar->model, model);
    newCar->type = CAR;
    newCar->wheels.pressure = pressure;
    newCar->wheels.width = width;
    return newCar;
}

void addLast(Vehicle* vehicle) 
{
    size++;
    if (head == NULL) 
    {
        head = vehicle;
        head->next = NULL;
        tail = head;
    }
    else
    {
        tail->next= vehicle;
        tail = vehicle;
    }
}

void removeAt(int index)
{
    if (size == 1)
        head = tail = NULL;
    else if (index==0)
    {
        head = head->next;
    }
    else
    {
        Vehicle* prev = head;
        for (int i = 0; i < index - 1; i++)
            prev = prev->next;
        Vehicle* element = prev->next;
        prev->next = element->next;
        if (index == size - 1)
            tail = prev;
        delete element;
    }
    size--;
}

void writeToFile() 
{
    std::ofstream out;
    out.open("bd.txt");
    if (out.is_open())
    {
        out << size << endl;
        Vehicle* now = head;
        for (int i = 0; i < size; i++)
        {
            out << now->model << endl;
            if (now->type == BOAT)
            {
                out << 0<< endl;
                out << now->deck.area << endl;
            }
            else
            {
                out << 1 << endl;
                out << now->wheels.pressure << endl;
                out << now->wheels.width << endl;
            }
            now = now->next;
        }
    }
    out.close();
}

void readFromFile()
{
    while (size != 0) 
        removeAt(0);

    std::ifstream in("bd.txt");
    if (in.is_open())
    {
        int nowSize;
        in >> nowSize;
        for (int i = 0; i < nowSize; i++)
        {
            char model[20];
            in >> model;
            int type;
            in >> type;
            if (type==0) {
                float value;
                in >> value;
                addLast(createBoat(model, value));
            }
            else if (type==1)
            {
                double value1;
                in >> value1;
                int value2;
                in >> value2;
                addLast(createCar(model, value1, value2));
            }
        }
    }
    in.close();
}

int main()
{
    setlocale(LC_ALL, "Rus");
    addLast(createBoat("Stormline", 3.3));
    addLast(createCar("Mercedes-Benz", 1.2, 3));

    cout << "Типо работа с бд:" << endl;

    while (true)
    {
        cout << "0 - выести все элементы; 1 - добавить элемент, 2 -удалить элемент; 3 - запись в файл ; 4 - чтение из файла; всё остальное - выход" << endl;
        int op;
        cin >> op;
        if (cin.fail())
            goto label;

        switch (op)
        {
        case 0:
            printAll();
            break;
        case 1:
            cout << "0 - лодка; 1 - машина; всё остальное - выход" << endl;
            cin >> op;
            if (cin.fail())
                goto label;
            if (op == 0)
            {
                cout << "Модель и площадь палубы:" << endl;
                char model[20];
                cin >> model;
                float area;
                cin >> area;
                addLast(createBoat(model, area));
            }
            else if (op == 1)
            {
                cout << "Модель, давление и ширина колёс:" << endl;
                char model[20];
                cin >> model;
                double pressure;
                cin >> pressure;
                int width;
                cin >> width;
                addLast(createCar(model, pressure, width));
            }
            else
                goto label;
            break;
        case 2:
            cout << "Кого?" << endl;
            cin >> op;
            if (cin.fail() || op < 0 || op >= size)
                goto label;
            removeAt(op);
            break;
        case 3:
            writeToFile();
            break;
        case 4:
            readFromFile();
            break;
        default:
            goto label;
        }
    }
label:
    cout << "The end" << endl;
}