#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QSerialPort *serial;
QByteArray ReceivedData;
int portstatus = 0;

QColor brush_color;
QColor background_color;
int draw_mode = 1;

char command_1[36] = "#1-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char command_2[36] = "#2-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char command_3[36] = "#3-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char command_4[36] = "#4-FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
char tmp[3] = "FF";
uint8_t frame_data[64] = {
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255,
    255,255,255,255
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Serial port
    serial = new QSerialPort(this);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
    //UI
    setFixedSize(555, 364);

    ui->setupUi(this);

    //Set brush colors
    background_color.setNamedColor("#555753");
    brush_color.setNamedColor("#FF0000");
    //Create Table
    int x = 0;
    int y = 0;
    while(y != 16){
        ui->LEDTable->insertRow(y);
        ui->LEDTable->setVerticalHeaderItem(y, new QTableWidgetItem(""));
        y++;
    }
    while(x != 32){
        ui->LEDTable->insertColumn(x);
        ui->LEDTable->setHorizontalHeaderItem(x, new QTableWidgetItem(""));
        x++;
    }
    x = 0;
    y = 0;
    while(y != 16){
        while(x != 32){
            ui->LEDTable->setItem(y, x, new QTableWidgetItem(""));
            ui->LEDTable->item(y,x)->setBackgroundColor(background_color);
            x++;
        }
        x = 0;
        y++;
    }

    ui->LEDTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}

void MainWindow::on_actionConnectButtonAction_triggered()
{
    uint16_t vid = 0;
    uint16_t pid = 0;
    int fr = 0;
    if (portstatus == 0){
        fr = 1;
        QString RPT;
        for (QSerialPortInfo port : QSerialPortInfo::availablePorts()){
            vid = port.vendorIdentifier();
            pid = port.productIdentifier();
            if (vid == 1155){
                if(pid == 22336){
                    RPT.clear();
                    RPT.append(port.portName());
                    serial->setPortName(RPT);
                    serial->setBaudRate(12000000);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    serial->setFlowControl(QSerialPort::NoFlowControl);
                    if((serial->open(QIODevice::ReadWrite) == true)){
                        ui->ConnectButton->setText("Disconnect");
                        portstatus = 1;
                        ui->StatusLabel->setText("Connected!");
                        fr = 2;
                        redrawFrame();
                    }
                }
            }
        }
    }
    else{
        serial->close();
        ui->ConnectButton->setText("Connect");
        portstatus = 0;
        ui->StatusLabel->setText("Disonnected!");
    }
    if (fr == 1){
        ui->StatusLabel->setText("Device not found!");
    }
}

void MainWindow::redrawFrame()
{
    int i = 0;
    while(i!=16){
        sprintf(&command_1[3+(i*2)], "%02x", frame_data[i]);
        i++;
    }
    i = 0;
    while(i!=16){
        sprintf(&command_2[3+(i*2)], "%02x", frame_data[i+16]);
        i++;
    }
    i = 0;
    while(i!=16){
        sprintf(&command_3[3+(i*2)], "%02x", frame_data[i+32]);
        i++;
    }
    i = 0;
    while(i!=16){
        sprintf(&command_4[3+(i*2)], "%02x", frame_data[i+48]);
        i++;
    }

    serial->write(command_1);
    serial->waitForBytesWritten();
    serial->write(command_2);
    serial->waitForBytesWritten();
    serial->write(command_3);
    serial->waitForBytesWritten();
    serial->write(command_4);
    serial->waitForBytesWritten();
}
void MainWindow::serialReceived()
{
    ReceivedData.append(serial->readAll());
    ReceivedData.clear();
}

void MainWindow::on_actionTableCellAction_triggered()
{
    QModelIndexList selcells = ui->LEDTable->selectionModel()->selectedIndexes();
    uint16_t cell_number = 0;
    int i = 0;
    while (i != selcells.count()){
        if(ui->LEDTable->item(selcells.at(i).row(),selcells.at(i).column())->backgroundColor() != (brush_color)){
            ui->LEDTable->item(selcells.at(i).row(),selcells.at(i).column())->setBackgroundColor(brush_color);
            cell_number = ((uint8_t)(selcells.at(i).row())*32)+(uint8_t)(selcells.at(i).column());
            if(draw_mode == 1){
                frame_data[(uint8_t)(cell_number/8)] &= ~(1<<(7-(cell_number%8)));
            }
            else{
                frame_data[(uint8_t)(cell_number/8)] |= (1<<(7-(cell_number%8)));
            }
        }
        i++;
    }
    if(portstatus == 1){
        redrawFrame();
    }
}

void MainWindow::on_actionDrawEraseToggleAction_triggered()
{
    if(draw_mode == 1){
        brush_color.setNamedColor("#555753");
        draw_mode = 0;
        ui->DrawEraseToggleButton->setText("Erase");
    }
    else{
        brush_color.setNamedColor("#FF0000");
        draw_mode = 1;
        ui->DrawEraseToggleButton->setText("Draw");
    }
}
