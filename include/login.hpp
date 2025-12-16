#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <conio.h>

using namespace std;

class LoginSystem
{
private:
    string userFile = "data/user.csv";
    string logFile = "data/console_log.csv";
    string lockFile = "data/account_locks.csv";

    unsigned int rightRotate(unsigned int value, unsigned int amount)
    {
        return (value >> amount) | (value << (32 - amount));
    }

    string sha256(const string &input)
    {
        unsigned int h0 = 0x6a09e667;
        unsigned int h1 = 0xbb67ae85;
        unsigned int h2 = 0x3c6ef372;
        unsigned int h3 = 0xa54ff53a;
        unsigned int h4 = 0x510e527f;
        unsigned int h5 = 0x9b05688c;
        unsigned int h6 = 0x1f83d9ab;
        unsigned int h7 = 0x5be0cd19;

        unsigned int k[64] = {
            0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
            0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
            0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
            0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
            0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
            0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
            0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
            0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
            0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
            0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
            0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
            0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
            0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb3,
            0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
            0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
            0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
        };

        string msg = input;
        unsigned long long bitLen = msg.size() * 8;

        msg += '\x80';
        while ((msg.size() % 64) != 56) msg += '\0';

        for (int i = 7; i >= 0; i--)
            msg += (bitLen >> (i * 8)) & 0xff;

        for (size_t chunk = 0; chunk < msg.size(); chunk += 64)
        {
            unsigned int w[64];

            for (int i = 0; i < 16; i++)
            {
                w[i] = (msg[chunk + i*4] & 0xff) << 24;
                w[i] |= (msg[chunk + i*4 + 1] & 0xff) << 16;
                w[i] |= (msg[chunk + i*4 + 2] & 0xff) << 8;
                w[i] |= (msg[chunk + i*4 + 3] & 0xff);
            }

            for (int i = 16; i < 64; i++)
            {
                unsigned int s0 = rightRotate(w[i-15], 7) ^ rightRotate(w[i-15], 18) ^ (w[i-15] >> 3);
                unsigned int s1 = rightRotate(w[i-2], 17) ^ rightRotate(w[i-2], 19) ^ (w[i-2] >> 10);
                w[i] = w[i-16] + s0 + w[i-7] + s1;
            }

            unsigned int a = h0;
            unsigned int b = h1;
            unsigned int c = h2;
            unsigned int d = h3;
            unsigned int e = h4;
            unsigned int f = h5;
            unsigned int g = h6;
            unsigned int h = h7;

            for (int i = 0; i < 64; i++)
            {
                unsigned int S1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
                unsigned int ch = (e & f) ^ (~e & g);
                unsigned int temp1 = h + S1 + ch + k[i] + w[i];
                unsigned int S0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
                unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
                unsigned int temp2 = S0 + maj;

                h = g;
                g = f;
                f = e;
                e = d + temp1;
                d = c;
                c = b;
                b = a;
                a = temp1 + temp2;
            }

            h0 += a;
            h1 += b;
            h2 += c;
            h3 += d;
            h4 += e;
            h5 += f;
            h6 += g;
            h7 += h;
        }

        stringstream ss;
        ss << hex << setfill('0')
           << setw(8) << h0
           << setw(8) << h1
           << setw(8) << h2
           << setw(8) << h3
           << setw(8) << h4
           << setw(8) << h5
           << setw(8) << h6
           << setw(8) << h7;

        return ss.str();
    }

    void writeLog(string username, string operation, string status, string reason)
    {
        time_t now = time(0);
        tm *lt = localtime(&now);

        char date[11];
        char timeStr[9];
        
        sprintf(date, "%04d-%02d-%02d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday);
        sprintf(timeStr, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);

        ofstream file(logFile, ios::app);
        file << date << "," << timeStr << "," << username << "," 
             << operation << "," << status << "," << reason << "\n";
        file.close();
    }

    bool validateUsername(string uname)
    {
        if (uname.length() < 4)
        {
            cout << "Username must be at least 4 characters!\n";
            return false;
        }

        bool hasLetter = false;
        bool allDigits = true;

        for (int i = 0; i < uname.length(); i++)
        {
            char c = uname[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                hasLetter = true;
                allDigits = false;
            }
            else if (c >= '0' && c <= '9')
            {
                // digit
            }
            else
            {
                cout << "Username can only contain letters and numbers!\n";
                return false;
            }
        }

        if (allDigits)
        {
            cout << "Username cannot be all numbers!\n";
            return false;
        }

        return true;
    }

    bool isAdminPassword(string pwd)
    {
        // Must be EXACTLY 10 characters: IDTB11 + 4 digits
        if (pwd.length() != 10) return false;
        
        // Check IDTB11 prefix (case-sensitive)
        if (pwd.substr(0, 6) != "IDTB11") return false;
        
        // Last 4 characters must be digits
        for (int i = 6; i < 10; i++)
        {
            if (pwd[i] < '0' || pwd[i] > '9')
            {
                return false;
            }
        }
        
        return true;
    }

    bool validatePassword(string pwd, bool isAdmin)
    {
        // Admin passwords have different requirements
        if (isAdmin)
        {
            // Admin password is just IDTB11XXXX (already validated)
            return true;
        }
        
        // Regular user password requirements
        if (pwd.length() < 8)
        {
            cout << "Password must be at least 8 characters!\n";
            return false;
        }

        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;
        bool hasSpecial = false;

        for (int i = 0; i < pwd.length(); i++)
        {
            char c = pwd[i];
            if (c >= 'A' && c <= 'Z') hasUpper = true;
            else if (c >= 'a' && c <= 'z') hasLower = true;
            else if (c >= '0' && c <= '9') hasDigit = true;
            else hasSpecial = true;
        }

        if (!hasUpper || !hasLower || !hasDigit || !hasSpecial)
        {
            cout << "Password must contain uppercase, lowercase, digit, and special character!\n";
            return false;
        }

        return true;
    }

public:
    bool usernameExists(string uname)
    {
        ifstream file(userFile);
        if (!file) return false;
        
        string line, savedName, savedHash, deleted, isAdmin;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedName, ',');
            getline(ss, savedHash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (savedName == uname && deleted != "1")
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    bool isAdmin(string uname)
    {
        ifstream file(userFile);
        if (!file) return false;
        
        string line, savedName, savedHash, deleted, isAdmin;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedName, ',');
            getline(ss, savedHash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (savedName == uname && deleted != "1")
            {
                file.close();
                return (isAdmin == "1");
            }
        }
        file.close();
        return false;
    }

    bool canChangePassword(string uname)
    {
        // Admins cannot change password
        return !isAdmin(uname);
    }

    bool isAccountLocked(string uname)
    {
        ifstream file(lockFile);
        if (!file) return false;
        
        string line, username, lockTimeStr;
        time_t now = time(0);

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, username, ',');
            getline(ss, lockTimeStr, ',');

            if (username == uname)
            {
                time_t lockTime = stoll(lockTimeStr);
                if (now - lockTime < 60)
                {
                    int remaining = 60 - (now - lockTime);
                    cout << "Account locked! Try again in " << remaining << " seconds.\n";
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    void lockAccount(string uname)
    {
        time_t now = time(0);
        ofstream file(lockFile, ios::app);
        file << uname << "," << now << "\n";
        file.close();
        cout << "\nToo many failed attempts! Account locked for 1 minute.\n";
    }

    string getHiddenPassword()
    {
        string password = "";
        char ch;

        while (true)
        {
            ch = _getch();

            if (ch == 13)
            {
                cout << endl;
                break;
            }
            else if (ch == 8)
            {
                if (password.length() > 0)
                {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                password += ch;
                cout << '*';
            }
        }

        return password;
    }

    bool signup(string uname, string pwd)
    {
        if (!validateUsername(uname))
        {
            writeLog(uname, "signup", "fail", "invalid_username_format");
            return false;
        }

        if (usernameExists(uname))
        {
            cout << "Username already exists!\n";
            writeLog(uname, "signup", "fail", "username_exists");
            return false;
        }

        bool adminAccount = isAdminPassword(pwd);
        
        if (!validatePassword(pwd, adminAccount))
        {
            writeLog(uname, "signup", "fail", "weak_password");
            return false;
        }

        string hashed = sha256(pwd);
        
        ofstream file(userFile, ios::app);
        file << uname << "," << hashed << ",0," << (adminAccount ? "1" : "0") << "\n";
        file.close();

        cout << "\nSignup successful!\n";
        if (adminAccount)
        {
            cout << "*** ADMIN ACCOUNT CREATED ***\n";
            cout << "Admin password is permanent and cannot be changed!\n";
        }
        
        writeLog(uname, "signup", "success", "none");
        return true;
    }

    bool login(string uname, string pwd)
    {
        ifstream file(userFile);
        if (!file)
        {
            cout << "Account not found!\n";
            writeLog(uname, "login", "fail", "account_not_found");
            return false;
        }

        string hashedInput = sha256(pwd);
        string line, savedName, savedHash, deleted, isAdmin;
        bool found = false;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedName, ',');
            getline(ss, savedHash, ',');
            getline(ss, deleted, ',');
            getline(ss, isAdmin, ',');

            if (savedName == uname && deleted != "1")
            {
                found = true;
                if (savedHash == hashedInput)
                {
                    file.close();
                    
                    if (isAdmin == "1")
                    {
                        cout << "\n*** ADMIN LOGIN SUCCESSFUL ***\n";
                    }
                    
                    writeLog(uname, "login", "success", "none");
                    return true;
                }
                else
                {
                    file.close();
                    cout << "Incorrect password!\n";
                    writeLog(uname, "login", "fail", "wrong_password");
                    return false;
                }
            }
        }

        file.close();
        if (!found)
        {
            cout << "Account not found!\n";
            writeLog(uname, "login", "fail", "account_not_found");
        }
        return false;
    }
};

#endif