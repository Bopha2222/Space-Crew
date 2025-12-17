#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <regex>
#include <ctime>


using namespace std;

class LoginSystem
{
private:

    string userFile = "user.csv";
    string logFile  = "console_log.csv";

    // ------------------------------------
    // SHA-256 (pure C++, NO OpenSSL needed)
    // ------------------------------------
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

    // ----------------------------
    // Write console log
    // ----------------------------
    void writeLog(const string &username,
                  const string &action,
                  const string &status)
    {
        time_t now = time(0);
        tm *lt = localtime(&now);

        int year   = lt->tm_year + 1900;
        int month  = lt->tm_mon + 1;
        int day    = lt->tm_mday;

        int hour   = lt->tm_hour;
        int minute = lt->tm_min;
        int second = lt->tm_sec;

        ofstream file(logFile, ios::app);

        file << username << ","
             << action << ","
             << status << ","
             << year << "-" << month << "-" << day << ","
             << hour << ":" << minute << ":" << second
             << "\n";
    }

    // ----------------------------
    // Check username exists
    // ----------------------------
    bool usernameExists(const string &uname)
    {
        ifstream file(userFile);

        string line;
        string savedName;
        string savedPwd;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, savedName, ',');
            getline(ss, savedPwd, ',');

            if (savedName == uname)
            {
                return true;
            }
        }

        return false;
    }

    // ----------------------------
    // Validate password strength
    // ----------------------------
    bool validatePassword(const string &pwd)
    {
        if (pwd.length() < 8) return false;

        regex upper("[A-Z]");
        regex lower("[a-z]");
        regex digit("[0-9]");
        regex special("[^A-Za-z0-9]");

        return regex_search(pwd, upper) &&
               regex_search(pwd, lower) &&
               regex_search(pwd, digit) &&
               regex_search(pwd, special);
    }

public:

    // ----------------------------
    // Signup
    // ----------------------------
    bool signup(const string &uname, const string &pwd)
    {
        if (usernameExists(uname))
        {
            cout << "Account already exists!\n";
            writeLog(uname, "signup", "fail_username_exists");
            return false;
        }

        if (!validatePassword(pwd))
        {
            cout << "Password too weak.\n";
            writeLog(uname, "signup", "fail_password_rules");
            return false;
        }

        string hashed = sha256(pwd);

        ofstream file(userFile, ios::app);
        file << uname << "," << hashed << "\n";

        cout << "Signup successful!\n";
        writeLog(uname, "signup", "success");

        return true;
    }

    // ----------------------------
    // Login
    // ----------------------------
    bool login(const string &uname, const string &pwd)
    {
        ifstream file(userFile);

        string hashedInput = sha256(pwd);
        string line;

        string savedName;
        string savedHash;

        while (getline(file, line))
        {
            stringstream ss(line);

            getline(ss, savedName, ',');
            getline(ss, savedHash, ',');

            if (savedName == uname)
            {
                if (savedHash == hashedInput)
                {
                    writeLog(uname, "login", "success");
                    return true;
                }
                else
                {
                    cout << "Incorrect password.\n";
                    writeLog(uname, "login", "fail_wrong_password");
                    return false;
                }
            }
        }

        cout << "Account not found.\n";
        writeLog(uname, "login", "fail_no_account");

        return false;
    }
};

#endif