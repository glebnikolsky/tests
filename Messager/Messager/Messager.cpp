#include <iostream>
#include <string>

using namespace std;

//void SendSms(const string& number, const string& message);
//void SendEmail(const string& email, const string& message);
void SendSms(const string& number, const string& message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string& email, const string& message) {
    cout << "Send '" << message << "' to e-mail "  << email << endl;
}


class INotifier{
public:
    virtual void Notify(const string& msg);
};

class SmsNotifier: public INotifier{
public:
    SmsNotifier(const string & phone_number):phone_(phone_number){

    }

    void Notify(const string& msg){
        SendSms(phone_, msg);
    }

protected:
    string phone_;
};

class EmailNotifier: public INotifier
{
public:
    EmailNotifier(const string &addr):email_address_(addr){

    }

    void Notify(const string& msg){
        SendEmail(email_address_, msg);
    }

protected:
    string email_address_;
};


void Notify(INotifier& notifier, const string& message) {
    notifier.Notify(message);
}

int main() {
    SmsNotifier sms{"+7-495-777-77-77"};
    EmailNotifier email{"na-derevnyu@dedushke.ru"};

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}