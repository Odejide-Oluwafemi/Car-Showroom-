/*
    Author: Odejide Oluwafemi

    Description:
        This was supposed to be a mini project demo on how static components
        work but, one thing leading to another, I got carried away and boom,
        here's where we at!

*/

//#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

/*
Class Components declared as [static] are shared between objects of the
class. That is, they exist throughout the whole program and they are created
ONCE.
*/

// A use of static components is to keep track of how many
// instances of a class has been created.

string removeSpaces(string text)
{
    string newText;

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == ' ')
            continue;
        newText.append(1, text[i]);
    }

    return newText;
}


class User
{
    protected:
        string email, pass;
    
    public:
        bool operator !()
        {
            return email == "";
        }

        User(string e, string p)
        {
            email = e;
            pass  = p;
        };

        string getEmail()
        {
            return email;
        }

        string getPass()
        {
            return pass;
        }

        void setEmail(string e)
        {
            email = e;
        }

        void setPass(string p)
        {
            pass = p;
        }

        virtual bool changeEmail(string newMail) = 0;
        virtual void changePass(string newPass) = 0;
        virtual void deleteAccount() = 0;

};

class Car
{
    private:
        string name, color, regNo;
        float speed;
        double price;

    public:
        string owner;
        static int InstanceCount;
        static string GenerateRegNo()
        {
            char output[9];
            srand(time(NULL));
            // a = 97, z = 122, A = 65, Z = 90, 0 = 48, 9 = 57
            for (int i = 0; i < 3; i++)
            {
                char r = (rand() % 24) + 65;
                output[i] = r;
            }
            output[3] = '-';

            for (int i = 4; i < 7; i++)
            {
                char r = (rand() % 9) + 48;
                output[i] = r;
            }
            
            return output;
        }

        Car(string n, string c, float s, double p, string rN = "")
        {

            name = n;
            color = c;
            speed = s;
            price = p;
            ++InstanceCount;
            regNo = rN == "" ? Car::GenerateRegNo() : rN;
        }

        Car()
        {}

        Car(Car const &source)
        {
            owner = source.owner;
            name = source.name;
            color = source.color;
            speed = source.speed;
            price = source.price;
            regNo = source.regNo;
            ++InstanceCount;
        }


        ~Car()
        {
            --InstanceCount;
        }

        int getInstanceCount()
        {
            return InstanceCount;
        }

        string getName()
        {
            return name;
        }

        string getRegNo()
        {
            return regNo;
        }

        string getColor()
        {
            return color;
        }

        float getSpeed()
        {
            return speed;
        }

        double getPrice()
        {
            return price;
        }

        // Here, the parameter name [name] clashes with the private
        // field name [name] so to refer to the field, it must be identified...
        void setName(string name)
        {
            
            // ...either by the [this] pointer and the arrow notation...
            this->name = name;
        }

        void setColor(string color)
        {
            // ...or by dereferencing the [this] pointer and the dot [.] operator
            (*this).color = color;
        }

        void setSpeed(float speed)
        {
            // ...or the class name and the Scope-Resolution [::] operator.
            Car::speed = speed;
        }

        void setPrice(double price)
        {
            this->price = price;
        }

        void setRegNo(string rN)
        {
            regNo = rN;
        }
};

class Admin : public User
{
    private:
        vector<Car> cars;

        void PopulateList()
        {
            cars.clear();
            ifstream file("allcars.txt");
            string o, rn, n, c;
            double pr;
            float s;
            file >> o >> rn >> n >> pr >> s >> c;
            while(!file.eof())
            {
                if (o == email)
                {
                    Car car(n, c, s, pr, rn);
                    car.owner = email;
                    cars.push_back(car);
                }
                file >> o >> rn >> n >> pr >> s >> c;
            }
            file.close();
        }

    public:
        Admin(string e, string p) : User(e, p)
        {
            PopulateList();
        };

        bool changeEmail(string newMail)
        {
            string email = User::getEmail();
            string pass = User::getPass();

            // Check First
            ifstream check("admin.txt");
            string cem, cp;
            check >> cem >> cp;
            while (!check.eof())
            {
                if (cem == newMail)
                {
                    cout << "This Email already Exists!\n";
                    check.close();
                    return false;
                }
                check >> cem >> cp;
            }
            check.close();

            string e, p;
            fstream file1("admin1.txt", ios::app | ios::out);
            ifstream file("admin.txt", ios::in);

            file >> e >> p; // >> oldPass
            while(!file.eof())
            {
                if (email == e)
                {
                    file1 << newMail << " " << p << endl;
                }
                else
                {
                    file1 << e << " " << p << endl;
                }
                file >> e >> p;
            }

            file.close();
            file1.close();

            remove("admin.txt");
            rename("admin1.txt", "admin.txt");

            User::setEmail(newMail);

            ifstream rAllCars("allcars.txt");
            ofstream wAllCars("allcars1.txt", ios::out | ios::app);

            string m, rn, cn, cpr, cs, cc;
            rAllCars >> m >> rn >> cn >> cp >> cs >> cc;
            while(!rAllCars.eof())
            {
                if (m == email)
                    wAllCars << newMail << " " << rn << " " << cn << " " << cpr << " " << cs << " " << cc << endl;
                else
                    wAllCars << m << " " << rn << " " << cn << " " << cpr << " " << cs << " " << cc << endl;

                rAllCars >> m >> rn >> cn >> cpr >> cs >> cc;
            }

            wAllCars.close();
            rAllCars.close();

            remove("allcars.txt");
            rename("allcars1.txt", "allcars.txt");

            return true;
        }

        void changePass(string newPass)
        {
            string email = User::getEmail();
            string oldPass = User::getPass();

            ifstream file("admin.txt", ios::in);
            ofstream file1("admin1.txt", ios::app | ios::out);

            string m, p;

            file >> m >> p;
            while(!file.eof())
            {
                if (m == email && p == oldPass)
                {
                    file1 << m << " " << newPass << endl;
                }
                else
                {
                    file1 << m << " " << p << endl;
                }
                file >> m >> p;
            }

            file1.close();
            file.close();

            remove("admin.txt");
            rename("admin1.txt", "admin.txt");

            User::setPass(newPass);
        }

        void addCar()
        {
            PopulateList();
            system("cls");
            // n c s p
            string name, color, regNo;
            float speed;
            double price;
            cout << "===========================\n";
            cout << "          Add a Car        \n";
            cout << "===========================\n";
            cout << "\n";

            cin.ignore();
            cout << "Car Name:  ";  getline(cin, name);
            cout << "Car Price: ";  cin >> price;
            cout << "Car Speed: ";  cin >> speed;
            cout << "Car Color: ";  cin >> color;
            cout << endl;

            Dup:
            bool duplicate = false;
            Car c = Car(name, color, speed, price);
            c.owner = getEmail();
            for (int i = 0; i < cars.size(); i++)
            {
                if (cars.at(i).getRegNo() == c.getRegNo())
                {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate)
                goto Dup;

            cars.push_back(c);
            cout << "Car Added Successfully\n";
            cout << "Registration Number: " << c.getRegNo() << "\n\n";

            ofstream allCar("allcars.txt", ios::out | ios::app);
            allCar << getEmail() << " " << c.getRegNo() << " " << removeSpaces(c.getName()) << " " << price << " " << speed << " " << color << endl;
            allCar.close();

        }

        void removeCar()
        {
            PopulateList();
            if (cars.empty())
            {
                cout << "There are no cars on sale to remove.\n\n";
                return;
            }

            cout << "===========================\n";
            cout << "        Remove a Car       \n";
            cout << "===========================\n";
            cout << "\n";

            cout << "Enter Registration Number: ";
            string regCheck;    cin >> regCheck;

            if (showCar(regCheck))
            {
                Confirm:
                cout << "Confirm Deletion(Y/N): ";
                char confirm;   cin >> confirm;

                switch(confirm)
                {
                    case 'y': case 'Y':
                        {
                            vector<Car> newVec;
                            vector<Car>::iterator it;
                            for (it = cars.begin(); it != cars.end(); ++it)
                            {
                                if (it->getRegNo() == regCheck)
                                    continue;
                                
                                newVec.push_back(*it);
                            }
                            cars.clear();
                            cars = newVec;
                            cout << regCheck << " has been deleted successfully!\n\n";

                            ifstream rAllCars("allcars.txt", ios::in);
                            if (!rAllCars)
                            {
                                cout << "Something is Brutally Wrong! Contact our Software HelpDest Immediately!\n\n";
                                break;
                            }

                            ofstream wAllCars("allcars1.txt", ios::out | ios::app);
                            string em, rn, sn, p, s, c;
                            rAllCars >> em >> rn >> sn >> p >> s >> c;
                            while (!rAllCars.eof())
                            {
                                if (!(regCheck == rn))
                                    wAllCars << em << " " << rn << " " << sn << " " << p << " " << s << " " << c << endl;
                                rAllCars >> em >> rn >> sn >> p >> s >> c;
                            }
                            rAllCars.close();
                            wAllCars.close();

                            remove("allcars.txt");
                            rename("allcars1.txt", "allcars.txt");

                            break;
                        }
                    case 'n': case 'N':
                        cout << "Action Cancelled!\n\n";
                        break;
                    default:
                        cout << "Invalid Input!" << endl << endl;
                        goto Confirm;
                }

            }

        }

        void editCar()
        {
            PopulateList();
            if (cars.empty())
            {
                cout << "There are no cars on sale to edit.\n\n";
                return;
            }
            cout << "===========================\n";
            cout << "         Edit a Car        \n";
            cout << "===========================\n";
            cout << "\n";

            cout << "1| Proceed\n";
            cout << "2| Back\n";

            Proceed:
            char ch;    cin >> ch;
            switch(ch)
            {
                case '1':
                    break;
                case '2':
                    return;
                    break;
                default:
                    cout << "Invalid Option!\n\n";
                    goto Proceed;
            }

            cout << "Enter Registration Number: ";
            string regCheck;    cin >> regCheck;
            system("cls");
            if (showCar(regCheck))
            {
                Car *c = getCar(regCheck);
                cout << "What do you want to Edit?\n\n";
                cout << "1| Name\n";
                cout << "2| Price\n";
                cout << "3| Speed\n";
                cout << "4| Color\n";
                cout << "5| Exit\n";

                cout << "\nInput Choice: ";
                char edit;  cin >> edit;
                switch(edit)
                {
                    case '1':
                        {
                            string newName;
                            cout << "Enter the new Name: ";
                            cin.ignore();
                            getline(cin, newName);
                            c->setName(newName);
                            cout << "Name set Successfully!\n\n";
                        }
                        break;
                    
                    case '2':
                        {
                            double newPrice;
                            cout << "Enter the new Price: ";
                            cin >> newPrice;
                            c->setPrice(newPrice);
                            cout << "Price set Successfully!\n\n";
                        }
                        break;

                    case '3':
                        {
                            float newSpeed;
                            cout << "Enter the new Speed: ";
                            cin >> newSpeed;
                            c->setSpeed(newSpeed);
                            cout << "Speed set Successfully!\n\n";
                        }
                        break;

                    case '4':
                        {
                            string newColor;
                            cout << "Enter the new Color: ";
                            cin >> newColor;
                            c->setColor(newColor);
                            cout << "Color set Successfully!\n\n";
                        }
                        break;
                    
                    case '5':
                        return;

                    default:
                        cout << "Invalid Input!\n";             
                }

                ifstream rAllCars("allcars.txt", ios::in);
                ofstream wAllCars("allcars1.txt", ios::out | ios::app);

                string em, rn, sn, p, s, cl;
                rAllCars >> em >> rn >> sn >> p >> s >> cl;

                while(!rAllCars.eof())
                {
                    if (regCheck == rn)
                    {
                        wAllCars << c->owner << " " << c->getRegNo() << " " << removeSpaces(c->getName()) << " " << c->getPrice() << " " << c->getSpeed() << " " << c->getColor() << endl;
                    }
                    else
                    {
                        wAllCars << em << " " << rn << " " << sn << " " << p << " " << s << " " << cl << endl;
                    }

                    rAllCars >> em >> rn >> sn >> p >> s >> cl;
                }
                delete c;
                rAllCars.close();
                wAllCars.close();

                remove("allcars.txt");
                rename("allcars1.txt", "allcars.txt");

            }
            editCar();
        }

        Car* getCar(string regNo)
        {
            PopulateList();
            for (int i = 0; i < cars.size(); i++)
            {
                if (cars[i].getRegNo() == regNo)
                    return &cars[i];
            }
            
        }

        bool showCar(string regNo)
        {
            PopulateList();
            bool found = false;
            vector<Car>::iterator it;
            Car c;
            for (it = cars.begin(); it != cars.end(); ++it)
            {
                if (it->getRegNo() == regNo)
                {
                    c = *it;
                    found = true;
                    break;
                }
                else
                {
                    cout << "No Car found with RegNo: " << regNo << "\n\n";
                    found = false;
                }
            }

            if (!found)
                return found;

            system("cls");
            cout << "============================\n";
            cout << "       Car Information      \n";
            cout << "============================\n";

            cout << setfill(' ') << setw(9) << "RegNo|";
            cout << setfill(' ') << setw(21) << "Name|";
            cout << setfill(' ') << setw(10) << "Price|";
            cout << setfill(' ') << setw(9) << "Speed|";
            cout << setfill(' ') << setw(9) << "Color|";
            cout << "\n\n";

            cout << setfill(' ') << setw(8)  << c.getRegNo() << "|";
            cout << setfill(' ') << setw(20) << c.getName()  << "|";
            cout << setfill(' ') << setw(9)  << c.getPrice() << "|";
            cout << setfill(' ') << setw(8)  << c.getSpeed() << "|";
            cout << setfill(' ') << setw(8)  << c.getColor() << "|";
            cout << endl;

            return found;
        }

        void showCars()
        {
            PopulateList();
            system("cls");
            cout << "============================\n";
            cout << "      Cars on Display       \n";
            cout << "============================\n";

            if (cars.empty())
            {
                cout << "There are no cars on display!" << endl << endl;
                return;
            }

            cout << setfill(' ') << setw(9) << "RegNo|";
            cout << setfill(' ') << setw(21) << "Name|";
            cout << fixed << setprecision(2) << setfill(' ') << setw(10) << "Price|";
            cout << setfill(' ') << setw(9) << "Speed|";
            cout << setfill(' ') << setw(9) << "Color|";
            cout << "\n\n";


            int count = 1;
            vector<Car>::iterator it;
            for (it = cars.begin(); it != cars.end(); ++it)
            {
                cout << setfill(' ') << setw(8)  << it->getRegNo() << "|";
                cout << setfill(' ') << setw(20) << it->getName()  << "|";
                cout << setfill(' ') << setw(9)  << it->getPrice() << "|";
                cout << setfill(' ') << setw(8)  << it->getSpeed() << "|";
                cout << setfill(' ') << setw(8)  << it->getColor() << "|";
                cout << endl;
                count++;
            }

            cout << "\n";
        }

        void editAccount()
        {
            system("cls");
            EditAcc:
            cout << "======================\n";
            cout << "     Edit Account     \n";
            cout << "======================\n";
            cout << endl;

            cout << "1| Change Email" << endl;
            cout << "2| Change Password" << endl;
            cout << "3| Exit" << endl;
            cout << "\tInput Choice: ";
            char choice;    cin >> choice;

            switch(choice)
            {
                case '1':
                    {
                        cout << "Enter New Email: ";
                        string newMail;
                        cin >> newMail;
                        cout << "Are you satisfied with " << newMail << "? (Y/N) ";
                        char satisfied; cin >> satisfied;

                        switch (satisfied)
                        {
                            case 'Y': case 'y':
                                if (changeEmail(newMail))
                                    cout << "Email Changed Successfully\n";
                                else
                                    cout << "An Error Occured!\n";
                                break;
                            
                            case 'N' : case 'n':
                                editAccount();
                                break;
                            
                            default:
                                cout << "Invalid Input\n";
                                editAccount();

                        }

                        break;
                    }
                
                case '2':
                    {
                        cout << "Enter Old Password: ";
                        string oldPass;
                        cin >> oldPass;

                        cout << "Enter New Password: ";
                        string newPass;
                        cin >> newPass;

                        cout << "Confirm New Password: ";
                        string confirmPass;
                        cin >> confirmPass;

                        if (oldPass != User::getPass())
                        {
                            cout << "Incorrect Password!\n";
                            goto EditAcc;
                        }
                        else if(newPass != confirmPass)
                        {
                            cout << "New Passwords do not match!\n";
                            goto EditAcc;
                        }

                        cout << "Are you satisfied with " << newPass << "? (Y/N) ";
                        char satisfied; cin >> satisfied;

                        switch (satisfied)
                        {
                            case 'Y': case 'y':
                                changePass(newPass);
                                cout << "Password Changed Successfully\n";
                                break;
                            
                            case 'N' : case 'n':
                                goto EditAcc;
                                break;
                            
                            default:
                                cout << "Invalid Input\n";
                                goto EditAcc;
                        }
                        
                        break;
                    }
                
                case '3':
                    return;

                default:
                    goto EditAcc;
                    
            }
            goto EditAcc;
        }

        void deleteAccount()
        {
            system("cls");
            cout << "============================\n";
            cout << "       Delete Account       \n";
            cout << "============================\n";
            cout << "\n";

            Del:
            cout << "Are you sure you want to PERMANENTLY ERASE YOUR DATA?(Y/N): ";
            char del;   cin >> del;
            switch(del)
            {
                case 'y' : case 'Y':
                    {
                        cout << "Enter Password: ";
                        string input;   cin >> input;

                        ifstream file("admin.txt", ios::in);

                        string fEmail, fPass;

                        bool correct = false;

                        file >> fEmail >> fPass;
                        while(!file.eof())
                        {
                            if (fEmail == getEmail() && fPass == input)
                            {
                                correct = true;
                                break;
                            }
                            file >> fEmail >> fPass;
                        }
                        file.close();


                        if (!correct)
                        {
                            cout << "Incorrect Password!\n\n";
                            goto Del;
                        }

                        file.open("admin.txt", ios::in);
                        ofstream file1("admin1.txt", ios::out | ios::app);

                        file >> fEmail >> fPass;
                        while(!file.eof())
                        {
                            if (!(fEmail == getEmail() && fPass == getPass()))
                            {
                                file1 << fEmail << " " << fPass << endl;
                            }
                            file >> fEmail >> fPass;
                        }

                        file1.close();
                        file.close();

                        remove("admin.txt");
                        rename("admin1.txt", "admin.txt");

                        ifstream rAllCars("allcars.txt", ios::in);
                            if (!rAllCars)
                            {
                                cout << "Something is Brutally Wrong! Contact our Software HelpDest Immediately!\n\n";
                                break;
                            }

                            ofstream wAllCars("allcars1.txt", ios::out | ios::app);
                            string em, rn, sn, p, s, c;
                            rAllCars >> em >> rn >> sn >> p >> s >> c;
                            while (!rAllCars.eof())
                            {
                                if (!(getEmail() == em))
                                    wAllCars << em << " " << rn << " " << sn << " " << p << " " << s << " " << c << endl;
                                rAllCars >> em >> rn >> sn >> p >> s >> c;
                            }
                            rAllCars.close();
                            wAllCars.close();

                            remove("allcars.txt");
                            rename("allcars1.txt", "allcars.txt");

                        delete this;

                        
                    }
                    break;

                case 'n' : case 'N':
                    return;
                    break;

                default:
                    cout << "Invalid Option!\n\n";
                    deleteAccount();
            }

        }
};

class Guest : public User
{
    public:
        Guest(string e, string p) : User(e, p){};

        bool changeEmail(string newMail)
        {
            string email = User::getEmail();
            string pass = User::getPass();

            // Check First
            ifstream check("guest.txt");
            string cem, cp;
            check >> cem >> cp;
            while (!check.eof())
            {
                if (cem == email)
                {
                    cout << "This Email already Exists!\n";
                    check.close();
                    return false;
                }
                check >> cem >> cp;
            }
            check.close();

            string e, p;
            fstream file1("guest1.txt", ios::app | ios::out);
            ifstream file("guest.txt", ios::in);

            file >> e >> p; // >> oldPass
            while(!file.eof())
            {
                if (email == e)
                {
                    file1 << newMail << " " << p << endl;
                }
                else
                {
                    file1 << e << " " << p << endl;
                }
                file >> e >> p;
            }

            file.close();
            file1.close();

            remove("guest.txt");
            rename("guest1.txt", "guest.txt");

            User::setEmail(newMail);
            return true;
        }

        void changePass(string newPass)
        {
            string email = User::getEmail();
            string oldPass = User::getPass();

            ifstream file("guest.txt", ios::in);
            ofstream file1("guest1.txt", ios::app | ios::out);

            string m, p;

            file >> m >> p;
            while(!file.eof())
            {
                if (m == email && p == oldPass)
                {
                    file1 << m << " " << newPass << endl;
                }
                else
                {
                    file1 << m << " " << p << endl;
                }
                file >> m >> p;
            }

            file1.close();
            file.close();

            remove("guest.txt");
            rename("guest1.txt", "guest.txt");

            User::setPass(newPass);
        }

        void deleteAccount()
        {
            system("cls");
            cout << "============================\n";
            cout << "       Delete Account       \n";
            cout << "============================\n";
            cout << "\n";

            Del:
            cout << "Are you sure you want to PERMANENTLY ERASE YOUR DATA?(Y/N): ";
            char del;   cin >> del;
            switch(del)
            {
                case 'y' : case 'Y':
                    {
                        cout << "Enter Password: ";
                        string input;   cin >> input;

                        ifstream file("guest.txt", ios::in);

                        string fEmail, fPass;

                        bool correct = false;

                        file >> fEmail >> fPass;
                        while(!file.eof())
                        {
                            if (fEmail == getEmail() && fPass == input)
                            {
                                correct = true;
                                break;
                            }
                            file >> fEmail >> fPass;
                        }
                        file.close();


                        if (!correct)
                        {
                            cout << "Incorrect Password!\n\n";
                            goto Del;
                        }

                        file.open("guest.txt", ios::in);
                        ofstream file1("guest1.txt", ios::out | ios::app);

                        file >> fEmail >> fPass;
                        while(!file.eof())
                        {
                            if (!(fEmail == getEmail() && fPass == getPass()))
                            {
                                file1 << fEmail << " " << fPass << endl;
                            }
                            file >> fEmail >> fPass;
                        }

                        file1.close();
                        file.close();

                        remove("guest.txt");
                        rename("guest1.txt", "guest.txt");

                        delete this;

                        
                    }
                    break;

                case 'n' : case 'N':
                    return;
                    break;

                default:
                    cout << "Invalid Option!\n\n";
                    deleteAccount();
            }

        }
};


// This has to be set outside the class because static components are
// actually not part of any object.
int Car::InstanceCount = 0;

void adminLogin();
void guestLogin();
void adminPanel(Admin admin);
void guestPanel(Guest guest);
bool login(string email, string password);
bool loginGuest(string email, string password);
bool registeration(string email, string password);
bool registerationGuest(string email, string password);


int main()
{
    MainMenu:
    system("cls");
    cout << "\n#Elyte_Tech#\n\n";
    // Code Starts Here
    
    char loginChoice;
    cout << "=====================================\n";
    cout << "  Welcome to #Elyte_Cars# Showroom.  \n";
    cout << "=====================================\n";
    cout << endl;

    cout << "        Select an Option:        \n";

    cout << "1. Admin\n";
    cout << "2. Guest\n";
    cout << "3. Exit Program\n";
    cout << "\tInput Choice: ";
    cin >> loginChoice;
    
    
    switch (loginChoice)
    {
        case '1':
            adminLogin();
            break;
        
        case '2':
            guestLogin();
            break;
        
        case '3':
            cout << "Thank you for using our services! See you next time.\n\n";
            exit(0);
        
        default:
            cout << "\tPlease Input a valid Option!\n";
            goto MainMenu;
    }
    goto MainMenu;
    // And Ends Here
    cout << "\n\n";
    return 0;
}

void adminLogin()
{
    char loginChoice;

    cout << "\n\t=============\n";
    cout << "\tAdministrator\n";
    cout << "\t=============\n";
    cout << "\n";

    MainMenu:
    cout << "\tSelect an Option:\n";
    cout << "1. Log In\n";
    cout << "2. Register\n";
    cout << "3. Back to Main Menu\n";
    
    cout << "\tInput Choice: ";
    cin >> loginChoice;

    switch(loginChoice)
    {
        case '1':
            {
                string email, password;
                cout << "\nInput Email: ";
                cin >> email;
                cout << "Input Password: ";
                cin >> password;
                cout << endl;
                if (login(email, password))
                {
                    system("cls");
                    cout << "=================\n";
                    cout << "Login Successful!\n";
                    cout << "=================\n";
                    adminPanel(Admin(email, password));
                }
                else
                {
                    cout << "An Error occured! Please try again or contact our Software HelpDesk\n\n";
                    goto MainMenu;
                }
            break;
            }
        
        case '2':
            {
                string email, password;
                cout << "\nInput Email: ";
                cin >> email;
                cout << "Input Password: ";
                cin >> password;
                cout << endl;
                if (registeration(email, password))
                {
                    system("cls");
                    cout << "============================\n";
                    cout << "Account Creation Successful!\n";
                    cout << "============================\n";
                    adminPanel(Admin(email, password));
                }
                else
                {
                    cout << "An Error occured! Please try again or contact our Software HelpDesk\n\n";
                    goto MainMenu;
                }
            break;
            }
        
        case '3':
            return;
            break;
        
        default:
            cout << "\tPlease Input a valid Option!\n";
            system("cls");
            goto MainMenu;
    }
}

bool login(string email, string password)
{
    ifstream file("admin.txt");
    if (!file)
        return false;
    
    string em, pass;
    while(file >> em >> pass)
    {
        if (em == email && pass == password)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
    
}

bool loginGuest(string email, string password)
{
    ifstream file("guest.txt");
    if (!file)
        return false;
    
    string em, pass;
    while(file >> em >> pass)
    {
        if (em == email && pass == password)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
    
}

bool registeration(string email, string password)
{
    ifstream rfile("admin.txt", ios::in);
    if (rfile)
    {
        string e;
        while (rfile >> e)
        {
            if (e == email)
            {
                cout << "Account already Exists!" << endl;
                rfile.close();
                return false;
            }
        }
    }
    rfile.close();

    ofstream wfile("admin.txt", ios::app | ios::out);
    wfile << email << " " << password << endl;
    wfile.close();
    return true;
}

bool registerationGuest(string email, string password)
{
    ifstream rfile("guest.txt", ios::in);
    if (rfile)
    {
        string e;
        while (rfile >> e)
        {
            if (e == email)
            {
                cout << "Account already Exists!" << endl;
                rfile.close();
                return false;
            }
        }
    }
    rfile.close();

    ofstream wfile("guest.txt", ios::app | ios::out);
    wfile << email << " " << password << endl;
    wfile.close();
    return true;
}

void adminPanel(Admin admin)
{
    MainMenu:
    cout << "Welcome! " << admin.getEmail() << endl << endl;
    cout << "What would you like to do today?\n";

    cout << "|1| Put up a car for sale" << endl;
    cout << "|2| Remove a car from sale" << endl;
    cout << "|3| Edit a car specs on sale" << endl;
    cout << "|4| View cars on sale" << endl;
    cout << "|5| Edit Account" << endl;
    cout << "|6| Logout" << endl;
    cout << "|7| Delete Account" << endl;
    cout << "\nSelect Option: ";
    char choice;    cin >> choice;

    switch(choice)
    {
        case '1':
            admin.addCar();
            break;
        
        case '2':
            admin.removeCar();
            break;
        
        case '3':
            admin.editCar();
            break;
        
        case '4':
            admin.showCars();
            break;
        
        case '5':
            admin.editAccount();
            break;
        
        case '6':
            return;
            break;
        
        case '7':
            admin.deleteAccount();
            return;
            break;
        
        default:
            cout << "Please Input a valid Option!\n";
    }
    goto MainMenu;
}

void guestLogin()
{
    char loginChoice;

    cout << "\n=============\n";
    cout << "     Guest     \n";
    cout << "===============\n";
    cout << "\n";

    MainMenu:
    cout << "\tSelect an Option:\n";
    cout << "1. Log In\n";
    cout << "2. Register\n";
    cout << "3. Back to Main Menu\n";
    
    cout << "\tInput Choice: ";
    cin >> loginChoice;

    switch(loginChoice)
    {
        case '1':
            {
                string email, password;
                cout << "\nInput Email: ";
                cin >> email;
                cout << "Input Password: ";
                cin >> password;
                cout << endl;
                if (loginGuest(email, password))
                {
                    system("cls");
                    cout << "=================\n";
                    cout << "Login Successful!\n";
                    cout << "=================\n";
                    break;
                    guestPanel(Guest(email, password));
                }
                else
                {
                    cout << "An Error occured! Please try again or contact our Software HelpDesk\n\n";
                    goto MainMenu;
                }
            break;
            }
        
        case '2':
            {
                string email, password;
                cout << "\nInput Email: ";
                cin >> email;
                cout << "Input Password: ";
                cin >> password;
                cout << endl;
                if (registerationGuest(email, password))
                {
                    system("cls");
                    cout << "============================\n";
                    cout << "Account Creation Successful!\n";
                    cout << "============================\n";
                    guestPanel(Guest(email, password));
                }
                else
                {
                    cout << "An Error occured! Please try again or contact our Software HelpDesk\n\n";
                    goto MainMenu;
                }
            break;
            }
        
        case '3':
            return;
            break;
        
        default:
            cout << "\tPlease Input a valid Option!\n";
            system("cls");
            goto MainMenu;
    }
}

void guestPanel(Guest guest)
{
    cout << "\n==========================\n";
    cout << "      Welcome! " << guest.getEmail() << "  \n";
    cout << "============================\n";
    cout << "\n";
}