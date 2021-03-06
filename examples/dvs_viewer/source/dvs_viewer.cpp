#include "../third_party/chameleon/source/background_cleaner.hpp"
#include "../third_party/chameleon/source/dvs_display.hpp"
#include "../third_party/dvs_sepia/source/dvs_sepia.hpp"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <iostream>

int main(int argc, char* argv[]) {
    // create the Qt Application
    QGuiApplication app(argc, argv);

    // register Chameleon types
    qmlRegisterType<chameleon::background_cleaner>("Chameleon", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::dvs_display>("Chameleon", 1, 0, "DvsDisplay");

    // pass the header properties to qml
    QQmlApplicationEngine application_engine;
    application_engine.rootContext()->setContextProperty("header_width", dvs_sepia::camera::width());
    application_engine.rootContext()->setContextProperty("header_height", dvs_sepia::camera::height());

    // load the view and setup the window properties for OpenGL renderring
    application_engine.loadData(
#include "dvs_viewer.qml"
    );
    auto window = qobject_cast<QQuickWindow*>(application_engine.rootObjects().first());
    {
        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        window->setFormat(format);
    }

    // retrieve pointers to the display generated by qml
    auto dvs_display = window->findChild<chameleon::dvs_display*>("dvs_display");

    // create the event handling pipeline
    auto camera = dvs_sepia::make_camera(
        [&](sepia::dvs_event dvs_event) { dvs_display->push(dvs_event); },
        [&](std::exception_ptr exception) {
            try {
                std::rethrow_exception(exception);
            } catch (const std::exception& exception) {
                std::cerr << exception.what() << std::endl;
            }
            app.quit();
        });

    // run the Qt Application
    return app.exec();
}
