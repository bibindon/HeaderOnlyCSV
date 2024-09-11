#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Shlwapi.h"
#pragma comment( lib, "Shlwapi.lib" ) 

class csv
{
public:

    static std::vector<std::vector<std::string> > Read(const std::string& filepath)
    {
        std::vector<std::vector<std::string> > csvData;
        int result = PathFileExists(filepath.c_str());
        if (result == 0)
        {
            return csvData;
        }

        std::ifstream ifs(filepath);
        std::string buff;
        std::string buffComma;
        int headerColSize = 0; // �擪�s�̗�
        int row = 0;
        bool doubleQuoteMode = false;
        std::vector<std::string> work;

        // csv�t�@�C���́u"�v�L���ň͂܂�Ă���΃Z�����ŉ��s���ł��邱�Ƃ��l������
        // �ȉ���3�̍s�͂ǂ��3��ƂȂ�B
        // a,b,c    �@
        // a,b,"c
        // d
        // e"       �A
        // a,"b",c  �B
        // �B�̂悤�Ɂu"�v�ň͂܂�Ă��邪���s���Ȃ��ꍇ������

        // ���s�܂œǂ�
        while (std::getline(ifs, buff))
        {
            std::istringstream iss(buff);

            // �J���}�܂œǂ�
            while (std::getline(iss, buffComma, ','))
            {
                // �擪�s��������񐔂��J�E���g����
                if (row == 0)
                {
                    headerColSize++;
                }
                trim(buffComma);
                // �u"�v�L��������
                if (buffComma.find('"') != std::string::npos)
                {
                    // �u"�v�L�����擪�ɂ���
                    if (buffComma.at(0) == '"')
                    {
                        // �u"�v�L�����Ō�ɂȂ��i�B�̃p�^�[���ł͂Ȃ��j
                        if (buffComma.find_last_of('"') != buffComma.size() - 1)
                        {
                            doubleQuoteMode = true;
                            std::string work2 = buffComma;
                            // �A�̂悤�ȁu"�v�ň͂܂�A���s���܂ޗv�f��ǂݎ��
                            while (std::getline(ifs, buff))
                            {
                                iss = std::istringstream(buff);
                                while (std::getline(iss, buffComma, ','))
                                {
                                    work2 = work2 + "\n" + buffComma;
                                    if (buffComma.find_last_of('"') == buffComma.size()-1)
                                    {
                                        doubleQuoteMode = false;
                                        break;
                                    }
                                }
                                if (doubleQuoteMode == false)
                                {
                                    buffComma = work2;
                                    break;
                                }
                            }
                        }
                    }
                }
                work.push_back(buffComma);
            }
            if (doubleQuoteMode == false)
            {
                csvData.push_back(work);
                work.clear();
                row++;
            }
        }

        // �s�̍Ō�̗v�f���Ȃ������ꍇ�A�󕶎���ǉ�����
        // �ȉ��̃f�[�^�͂ǂ����3�񂠂�Ƃ���B
        // a,b,c
        // a,b,
        // ���̂��߁A�󕶎��̃f�[�^��ǉ�����
        for (std::size_t i = 0; i < csvData.size(); ++i)
        {
            if (csvData.at(i).size() != headerColSize)
            {
                csvData.at(i).push_back("");
            }
        }

        return csvData;
    }

private:
    csv();

    static void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ));
    }

    static void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ).base(), s.end());
    }

    static void trim(std::string& s)
    {
        rtrim(s);
        ltrim(s);
    }

//    std::vector<std::vector<std::string> > m_csvData;
};

