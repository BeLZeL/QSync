
#include <QApplication>
#include <form.h>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QSync");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    Form w;
    if ( args.count() == 2 )
        w.setArgs(args.at(0), args.at(1));
    w.show();

    return app.exec();
}

/*
 * TODO DELETE EMPTY SUB DIR
 * TODO DELETE BAD SYMLINK
 *
 * ligne de commande
 */
