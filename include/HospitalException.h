#ifndef HOSPITALEXCEPTION_H
#define HOSPITALEXCEPTION_H

class HospitalException
{
protected:
    char m_message[200];

public:
    HospitalException();
    HospitalException(const char* message);
    HospitalException(const HospitalException& other);
    virtual ~HospitalException();

    HospitalException& operator=(const HospitalException& other);

    virtual const char* what() const;
};

#endif