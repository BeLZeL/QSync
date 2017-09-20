// Launch
// ./QSync /home/<user>/<src_directory> /home/<user>/<dest_directory> [ AUTO ]

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

    w.show();

    if ( args.count() == 2 )
        w.setArgs(args.at(0), args.at(1), "MANUAL");
    else if ( args.count() == 3 )
        w.setArgs(args.at(0), args.at(1), args.at(2));

    if ( ! w.launch_automatic_mode() )
        app.exit();

    return app.exec();
}

/*
 * TODO DELETE EMPTY SUB DIR
 * TODO DELETE BAD SYMLINK
 *
 * ligne de commande
 */
