#include "../third_party/chameleon/source/background_cleaner.hpp"
#include "../third_party/chameleon/source/blob_display.hpp"
#include "../third_party/chameleon/source/dvs_display.hpp"
#include "../third_party/sepia/source/sepia.hpp"
#include "../third_party/tarsier/source/replicate.hpp"
#include "../third_party/tarsier/source/track_blob.hpp"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join(
    {sepia::dirname(SEPIA_DIRNAME), "third_party", "sepia", "third_party", "event_stream", "examples", "dvs.es"});

/// blob contains the parameters of a Gaussian blob.
struct blob {
    float x;
    float y;
    float sigma_x_squared;
    float sigma_xy;
    float sigma_y_squared;
} __attribute__((packed));

int main(int argc, char* argv[]) {
    // read the header
    const auto header = sepia::read_header(sepia::filename_to_ifstream(filename));

    // create the Qt Application
    QGuiApplication app(argc, argv);

    // register Chameleon types
    qmlRegisterType<chameleon::background_cleaner>("Chameleon", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::dvs_display>("Chameleon", 1, 0, "DvsDisplay");
    qmlRegisterType<chameleon::blob_display>("Chameleon", 1, 0, "BlobDisplay");

    // pass the header properties to qml
    QQmlApplicationEngine application_engine;
    application_engine.rootContext()->setContextProperty("header_width", header.width);
    application_engine.rootContext()->setContextProperty("header_height", header.height);

    // load the view and setup the window properties for OpenGL renderring
    application_engine.loadData(
#include "blob_tracker.qml"
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

    // retrieve pointers to the displays generated by qml
    auto dvs_display = window->findChild<chameleon::dvs_display*>("dvs_display");
    auto blob_display = window->findChild<chameleon::blob_display*>("blob_display");

    // initialize the blob display
    const blob initial_blob{20, 120, 200, 0, 200};
    blob_display->insert(0, initial_blob);

    // create the event handling pipeline
    auto observable = sepia::make_observable<sepia::type::dvs>(
        sepia::filename_to_ifstream(filename),
        tarsier::make_replicate<sepia::dvs_event>(
            [&](sepia::dvs_event dvs_event) { dvs_display->push(dvs_event); },
            tarsier::make_track_blob<sepia::dvs_event, blob>(
                initial_blob.x,
                initial_blob.y,
                initial_blob.sigma_x_squared,
                initial_blob.sigma_xy,
                initial_blob.sigma_y_squared,
                0.99,
                0.999,
                [](sepia::dvs_event dvs_event,
                   float x,
                   float y,
                   float squared_sigma_x,
                   float sigma_xy,
                   float sigma_y_squared) -> blob {
                    return {x, y, sigma_x_squared, sigma_xy, sigma_y_squared};
                },
                [&](blob blob) { blob_display->update(0, blob); })),
        [](std::exception_ptr) {},
        []() { return true; });

    // run the Qt Application
    return app.exec();
}
