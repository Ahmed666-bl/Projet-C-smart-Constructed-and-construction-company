#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "rdv.h"
#include <QMessageBox>
#include <QMainWindow>
#include <QIntValidator>
#include<QSqlQueryModel>
#include <QTextDocument>

#include <QtPrintSupport/QPrinter>

#include<QFileDialog>
#include <QPdfWriter>
#include <QDate>
#include <QTextDocument>
#include <QTextStream>
#include <QSqlQueryModel>
#include <QObject>
#include <QPrintDialog>
#include <QDesktopServices>
#include <arduino.h>

Client C;
arduino Ard;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_id->setValidator(new QIntValidator(0, 9999999, this));
   ui->tab_client->setModel(C.afficher());
   ui->tableView_alerte->setModel(C.getAlertes());
   ui->tab_client->setModel(C.trierid());
   ui->comboBox->setModel(C.afficher());




   /////////////
   int ret=Ard.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< Ard.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<Ard.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(Ard.getserial(),SIGNAL(readyRead()),this,SLOT(update_temperature())); //permet de lancer le slot update_ suite a la reception du signal readyread


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::controlsaisie(){

    if ( ui->le_id->text().isEmpty() ||
         ui->le_id->text().toInt() == 0 ||

            !(ui->le_nom->text().contains(QRegExp("^[A-Za-z]+$"))) || //frme mn google ( regex )
            !(ui->le_prenom->text().contains(QRegExp("^[A-Za-z]+$"))) ||
            !(ui->le_adresse->text().contains(QRegExp("^[A-Za-z]+$"))) ||
            !(ui->le_email->text().contains(QRegExp("^[A-Za-z]+$"))) ||

            ui->le_num_tel->text().isEmpty() ||
            ui->le_num_tel->text().toInt() == 0 )
        return 0;
    else
        return 1;
}
bool MainWindow::controlsaisie1(){

    if ( ui->id->text().isEmpty() ||
         ui->id->text().toInt() == 0 ||
         ui->heure->text().isEmpty() ||
                  ui->heure->text().toInt() == 0 )



        return 0;
    else
        return 1;
}
void MainWindow::on_pb_ajouter_clicked()
{
    if (controlsaisie()){
    int id_client=ui->le_id->text().toInt();
    QString nom_client=ui->le_nom->text();
    QString prenom_client=ui->le_prenom->text();
    QString adresse_client=ui->le_adresse->text();
    int num_tel_client=ui->le_num_tel->text().toInt();
    QString email_client=ui->le_email->text();

    Client C(id_client ,nom_client ,prenom_client,adresse_client,num_tel_client,email_client);
    bool test=C.ajouter();
            QMessageBox msgBox;
               if(test)
               {
                   msgBox.setText("ajout avec succes");
               msgBox.exec();
               }
               else
               {
                   msgBox.setText("echec");
               msgBox.exec();
               }
               ui->tab_client->setModel(C.afficher());

}
    else {
            QMessageBox::critical(nullptr, QObject::tr("not ok"),
                        QObject::tr("veuiller remplir tous les champs correctement.\n"
                                    "Click cancel to exit."), QMessageBox::Cancel);
    }
    }





void MainWindow::on_le_supprimer_clicked()
{
    int id_client =ui->le_id->text().toInt();
            bool test=C.supprimer(id_client);

            if(test)
                QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("supression effectué .\n"
                                        "Click Ok to exit."), QMessageBox::Ok);
            else
                QMessageBox::critical(nullptr, QObject::tr("not ok"),
                            QObject::tr("échec suprresion.\n"
                                        "Click cancel to exit."), QMessageBox::Cancel);
            ui->tab_client->setModel(C.afficher());
}



void MainWindow::on_pb_modifier_clicked()
{
    int id_client=ui->le_id->text().toInt();
       QString nom_client=ui->le_nom->text();
       QString prenom_client=ui->le_prenom->text();
       QString adresse_client=ui->le_adresse->text();
       int num_tel_client=ui->le_num_tel->text().toInt();
       QString email_client=ui->le_email->text();

          Client C(id_client ,nom_client ,prenom_client,adresse_client,num_tel_client,email_client);


        bool test=C.modifierClient(id_client ,nom_client ,prenom_client,adresse_client,num_tel_client,email_client);
        if(test)
      {ui->tab_client->setModel(C.afficher());
      QMessageBox::information(nullptr, QObject::tr("Modifier avec succées "),
                        QObject::tr("invite modifiée.\n"
                                    "Click ok to exit."), QMessageBox::Ok);

      }
        else
            QMessageBox::critical(nullptr, QObject::tr("Modifier a echoué"),
                        QObject::tr("echec d'ajout !.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
}




void MainWindow::on_pb_trier_clicked()
{
    ui->tab_client->setModel(C.trierid());
}


void MainWindow::on_recherche_textChanged(const QString &arg1)
{
    Client C;



      int id_client= ui->recherche->text().toInt();

      C.recherche(ui->tab_client,id_client);

      if (ui->recherche->text().isEmpty())
      {

          ui->tab_client->setModel(C.afficher());

      }
   }








void MainWindow::on_pushButton_9_clicked()
{

        if (controlsaisie1()){
        int id_rdv=ui->id->text().toInt();
        QDate date_rdv=ui->le_date->date();

        int heure_rdv=ui->heure->text().toInt();
        int id_cll=ui->comboBox->currentText().toInt();

        Rdv R(id_rdv ,date_rdv ,heure_rdv ,id_cll);
        bool test=R.ajouter1();
                QMessageBox msgBox;
                   if(test)
                   {
                       msgBox.setText("ajout avec succes");
                   msgBox.exec();
                   }
                   else
                   {
                       msgBox.setText("echec");
                   msgBox.exec();
                   }
                   ui->tab_rdv->setModel(R.afficher1());

    }
        else {
                QMessageBox::critical(nullptr, QObject::tr("not ok"),
                            QObject::tr("veuiller remplir tous les champs correctement.\n"
                                        "Click cancel to exit."), QMessageBox::Cancel);
        }
        }



void MainWindow::on_pushButton_10_clicked()
{
    int id_rdv =ui->id->text().toInt();
            bool test=R.supprimer1(id_rdv);

            if(test)
                QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("supression effectué .\n"
                                        "Click Ok to exit."), QMessageBox::Ok);
            else
                QMessageBox::critical(nullptr, QObject::tr("not ok"),
                            QObject::tr("échec suprresion.\n"
                                        "Click cancel to exit."), QMessageBox::Cancel);
            ui->tab_rdv->setModel(R.afficher1());
}

void MainWindow::on_pushButton_11_clicked()
{
    int id_rdv=ui->id->text().toInt();
       QDate date_rdv=ui->le_date->date();

       int heure_rdv=ui->heure->text().toInt();

       int id_cll=ui->comboBox->currentText().toInt();

          Rdv R(id_rdv,date_rdv ,heure_rdv,id_cll);


        bool test=R.modifierRdv(id_rdv ,date_rdv ,heure_rdv);
        if(test)
      {ui->tab_rdv->setModel(R.afficher1());
      QMessageBox::information(nullptr, QObject::tr("Modifier avec succées "),
                        QObject::tr("invite modifiée.\n"
                                    "Click ok to exit."), QMessageBox::Ok);

      }
        else
            QMessageBox::critical(nullptr, QObject::tr("Modifier a echoué"),
                        QObject::tr("echec d'ajout !.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

}


void MainWindow::on_bouton_excel_clicked()
{

    QTableView *table;
     table = ui->tab_client;

     QString filters("CSV files (*.csv);;All files (*.*)");
     QString defaultFilter("CSV files (*.csv)");
     QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                        filters, &defaultFilter);
     QFile file(fileName);

     QAbstractItemModel *model =  table->model();
     if (file.open(QFile::WriteOnly | QFile::Truncate)) {
         QTextStream data(&file);
         QStringList strList;
         for (int i = 0; i < model->columnCount(); i++) {
             if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                 strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
             else
                 strList.append("");
         }
         data << strList.join(";") << "\n";
         for (int i = 0; i < model->rowCount(); i++) {
             strList.clear();
             for (int j = 0; j < model->columnCount(); j++) {

                 if (model->data(model->index(i, j)).toString().length() > 0)
                     strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                 else
                     strList.append("");
             }
             data << strList.join(";") + "\n";
         }
         file.close();
         QMessageBox::information(this,"Exporter To Excel","Exporter En Excel Avec Succées ");
     }
}

void MainWindow::on_pdf_clicked()
{QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tab_client->model()->rowCount();
    const int columnCount = ui->tab_client->model()->columnCount();
    QString TT = QDate::currentDate().toString("yyyy/MM/dd");
    out <<"<html>\n"
          "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << "<title>ERP - COMmANDE LIST<title>\n "
        << "</head>\n"
        "<body bgcolor=#ffffff link=#5000A0>\n"
        "<h1 style=\"text-align: center;\"><strong> *****LISTE DES  licence commerciale *****"+TT+" </strong></h1>"
        "<table style=\"text-align: center; font-size: 20px;\" border=1>\n "
          "</br> </br>";
    // headers
    out << "<thead><tr bgcolor=#d6e5ff>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->tab_client->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tab_client->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tab_client->isColumnHidden(column)) {
                QString data =ui->tab_client->model()->data(ui->tab_client->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
        "</body>\n"
        "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, nullptr);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;
}


///stat
void MainWindow::on_stat_clicked()
{

        QSqlQueryModel * model= new QSqlQueryModel();
        model->setQuery("select * from CLIENT where ADRESSE_CLIENT = 'ariana' ");
        float dispo1=model->rowCount();

        model->setQuery("select * from CLIENT where ADRESSE_CLIENT = 'tunis' ");
        float dispo=model->rowCount();

        float total=dispo1+dispo;
            QString a=QString("ariana . " +QString::number((dispo1*100)/total,'f',2)+"%" );
            QString b=QString("tunis .  "+QString::number((dispo*100)/total,'f',2)+"%" );
            QPieSeries *series = new QPieSeries();
            series->append(a,dispo1);
            series->append(b,dispo);
        if (dispo1!=0)
        {QPieSlice *slice = series->slices().at(0);
            slice->setLabelVisible();
            slice->setPen(QPen());}
        if ( dispo!=0)
        {
            QPieSlice *slice1 = series->slices().at(1);
            slice1->setLabelVisible();
        }

        QChart *chart = new QChart();


        chart->addSeries(series);
        chart->setTitle(" ADRESSE_CLIENT :Nb ADRESSE_CLIENT: "+ QString::number(total));
        chart->legend()->hide();


        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(1000,500);
        chartView->show();
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    int ID_cle =ui->comboBox->currentText().toInt();
    QString id_string=QString::number(ID_cle);
    QSqlQuery query;
    query.prepare("select * from CLIENT where ID_CLIENT='"+id_string+"'");


}
/*
void MainWindow::on_buzzer_clicked()
{
    ui->tab_client->setCurrentIndex(0); //tbdel l page//
}*/
//afffficher mess box kn fama alerte
/*
void MainWindow::update_temperature()
{
    qDebug()<<Ard.read_from_arduino(); // afficher louuuta
    if (Ard.read_from_arduino()=="")
        //
       {
        qDebug("alerte");
        QMessageBox::critical(nullptr, QObject::tr("flame"),
                    QObject::tr("alerte flame!.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

    }
}*/
//afffficher mess box kn fama alerte

 void MainWindow::update_temperature()
{
    if (Ard.read_from_arduino()!="")
        //
       {
        QMessageBox::critical(nullptr, QObject::tr("gaz"),
                    QObject::tr("alerte gaz!.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

        QSqlQuery query;
        query.prepare("INSERT INTO ALERTE_GAZ (DATE_A) "
                      "VALUES (:DATE_A)");
        query.bindValue(":DATE_A",QDateTime::currentDateTime().toString());

        query.exec();
        ui->tableView_alerte->setModel(C.getAlertes());


    }

}


void MainWindow::on_pb_alarm_clicked()
{
     Ard.write_arduino("1");//output
}



