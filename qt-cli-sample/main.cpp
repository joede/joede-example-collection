/* -*- Mode: C -*-
 * --------------------------------------------------------------------------
 * Copyright  (c) Dipl.-Ing. Joerg Desch <github@jdesch.de>
 * --------------------------------------------------------------------------
 * PROJECT: qt-cli-sample
 * MODULE.: main.cpp
 * AUTHOR.: Dipl.-Ing. Joerg Desch
 * CREATED: 27.03.2019 11:00:06 CET
 * LICENSE: GPL 3.0
 * --------------------------------------------------------------------------
 * DESCRIPTION:
 *
 * --------------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 * --------------------------------------------------------------------------
 */

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QStringList>


/*+=========================================================================+*/
/*|                   CONSTANTS AND VARIABLE DEFINITIONS                    |*/
/*`========================================================================='*/
//{{{

/* a sample heightwith it's default and limits.
 */
static int Height = 720;
static const int MIN_HEIGHT=480;
static const int MAX_HEIGHT=1080;

/* If true, show some more diagnostic messages.
 */
bool BeVerbose = false;

//}}}

/*+=========================================================================+*/
/*|                     IMPLEMENTATION OF THE FUNCTIONS                     |*/
/*`========================================================================='*/
//{{{
//}}}

/*+=========================================================================+*/
/*|                    MAIN ENTRANCE OF THE APPLICATION                     |*/
/*`========================================================================='*/
//{{{

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qt-cli-sample");
    QCoreApplication::setApplicationVersion("0.1");
    QString OutputFile;                                         // The filename to write too.
    QString InputFile;                                          // sample input file.
    bool rc;

    // prepare the commandline interface
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt sample to parse the command line.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", "Sample input file");        // the input image

    QCommandLineOption showDiagnosticOption("d", "Show debugging messages");        // -d to enable debugging output
    parser.addOption(showDiagnosticOption);

    QCommandLineOption heightOption(QStringList() << "H" << "height",           // -H <num>
                                      "Sample option with numeric parameter",
                                      "num");
    parser.addOption(heightOption);

    QCommandLineOption outputOption(QStringList() << "o" << "output",               // -o <filename>
                                    "Sample <filename> to write the result too.",
                                    "filename");
    parser.addOption(outputOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    BeVerbose = parser.isSet(showDiagnosticOption);
    if ( parser.isSet(heightOption) )
    {
        Height = parser.value(heightOption).toInt(&rc);
        if ( Height<MIN_HEIGHT || Height>MAX_HEIGHT )
        {
            qCritical("error: height option out of range!");
            return 2;
        }
        if ( BeVerbose )
            qDebug("info: Height = %d",Height);
    }
    if ( parser.isSet(outputOption) )
    {
        OutputFile = parser.value(outputOption);
        if ( OutputFile.isEmpty() )
        {
            qCritical("error: illegal output option!");
            return 2;
        }
        if ( BeVerbose )
            qDebug() << "info: OutputFile =" << OutputFile;
    }
    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)
    if ( args.size() == 1 )
    {
        InputFile = args.at(0);
        if ( InputFile.isEmpty() )
        {
            qCritical("error: input file (source) missing!");
            return 2;
        }
        if ( BeVerbose )
            qDebug() << "info: InputFile =" << InputFile;
    }
    else
    {
        // show the help page
        parser.showHelp(1);
    }

    return 0;
}

//}}}

/* ==[End of file]========================================================== */
