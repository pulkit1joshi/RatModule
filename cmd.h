// ----------------CLASS DEFINATION OF COMMANDS WE SAVE-----------------------

#ifndef CMD_H
#define CMD_H
#include <QString>
#include <QDataStream>

class cmd
{

        public:
                    int num;
                    int type;
                    bool fields;
                    QString name;
                    QString cmdstr;
                    cmd(int n=0, QString nam="", QString postcm="",int type2 = 0)
                    {
                        num = n;
                        name = nam;
                        cmdstr = postcm;
                        type = type2;
                        fields = true;
                    }



                    friend QDataStream &operator<<(QDataStream &out, const cmd &command)
                    {
                                out << int(command.num) << command.name
                                    << command.cmdstr << int(command.type) << bool(command.fields);
                                return out;
                    }

                    friend QDataStream &operator>>(QDataStream &in, cmd &command)
                    {
                                QString name;
                                QString postcmd;
                                int type;
                                int num;
                                bool fields;
                                in >> num >> name >> postcmd >> type >> fields;
                                command = cmd(num,name,postcmd,type);
                                return in;
                    }

};


#endif // CMD_H
