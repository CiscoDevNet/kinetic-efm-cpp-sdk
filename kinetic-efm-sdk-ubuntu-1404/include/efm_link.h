// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_link.h

#pragma once

#include <efm_link_options.h>
#include <efm_requester.h>
#include <efm_responder.h>

#include <chrono>


namespace cisco
{
namespace efm_sdk
{

/// @brief This is the main entry point for a link implementation.

/// All access to the API will be channeled through an instance of this class. It holds the requester and responder
/// objects, depending on its configuration. Corresponding handlers can be supplied in order to be informed about
/// initialization, connect and disconnect events. It also gives access to the worker thread pool of the link via task
/// scheduling functions.
class Link final
{
public:
  /// Link initialization callback signature.
  /// @param link_name The name of this link. Either passed to the Link constructor or determined by the LinkOptions.
  /// @param ec Will be set to an error, if the initialization fails.
  using link_initialized_handler = std::function<void(const std::string& link_name, const std::error_code& ec)>;
  /// Link connected callback signature.
  /// @param ec Will be set to an error, if the connection has an error.
  using link_connected_handler = std::function<void(const std::error_code& ec)>;
  /// Link disconnected callback signature.
  /// @param ec Will be set to an error, if the connection has an error.
  using link_disconnected_handler = std::function<void(const std::error_code& ec)>;

  /// Constructs a Link instance. The cisco::efm_sdk::LinkType defines, if a requester or responder or both objects can
  /// be accessed. The options configure vital parts the links behaviour.
  /// @param options The options to configure the link with.
  /// @param link_type The type of the link. If it is a cisco::efm_sdk::LinkType::Requester, the requester object will
  /// be available. If it is a cisco::efm_sdk::LinkType::Responder, the responder object will be available. If it is
  /// cisco::efm_sdk::LinkType::RequesterResponder, both objects will be available.
  Link(LinkOptions&& options, LinkType link_type);

  /// Destroys the Link instance.
  ~Link();

  /// Returns the EFM cpp SDK version and build info.
  /// @return The EFM cpp SDK version and build info.
  const std::string& get_version_info() const;

  /// Returns the cisco::efm_sdk::LinkType.
  /// @return The cisco::efm_sdk::LinkType
  LinkType link_type() const;

  /// Checks if this Link is a responder.
  /// @return true if the responder object is available, otherwise false
  bool is_responder() const;

  /// Checks if this Link is a requester.
  /// @return true if the requester object is available, otherwise false
  bool is_requester() const;

  /// Starts the Link instance and blocks until the Link instance is shutdown. After the initialization is complete, the
  /// initialize callback handler will be called. Then the connect to the configured broker will be tried. If the link
  /// successfully connects to the broker, the connect callback handler will be called. Upon a disconnect, the
  /// disconnect callback handler will and a reconnect will be retried until successful or the link is shutdown. Every
  /// successful reconnect will fire the connect callback handler and every disconnect will fire the disconnect callback
  /// handler.
  void run();

  /// Starts the Link instance. This method is non blocking and you should use it if you have to call a blocking loop
  /// after you start the link. To shutdown the link instance you must call to stop() and join(). After the
  /// initialization is complete, the initialize callback handler will be called. Then the connect to the configured
  /// broker will be tried. If the link successfully connects to the broker, the connect callback handler will be
  /// called. Upon a disconnect, the disconnect callback handler will and a reconnect will be retried until successful
  /// or the link is shutdown. Every successful reconnect will fire the connect callback handler and every disconnect
  /// will fire the disconnect callback handler.
  ///
  /// @code
  ///     ResponderLink responder_link(link);
  ///     responder_link.non_blocking_run();
  ///
  ///     start_some_blocking_loop_function();
  ///
  ///     responder_link.stop();
  ///     responder_link.join();
  /// @endcode
  void non_blocking_run();

  /// Stop the link instance.
  void stop();

  /// This method must be called after previous call to non_blocking_run() and stop(). The method will block until the
  /// link instance is shutdown.
  void join();

  /// Returns the requester object, if this Link is a requester. It will throw an exception if the link is not a
  /// requester.
  /// @throw cisco::efm_sdk::exception
  /// @return The requester object
  Requester& requester();

  /// Returns the responder object, if this Link is a requester. It will throw an exception if the link is not a
  /// responder.
  /// @throw cisco::efm_sdk::exception
  /// @return The responder object
  Responder& responder();

  /// Sets the initialized callback handler, which will be called only once upon call of the run method.
  /// @param handler The handler to call once upon initialization.
  void set_on_initialized_handler(link_initialized_handler&& handler);

  /// Sets the deinitialized callback handler, which will be called only once upon call of the run method.
  /// @param handler The handler to call once upon initialization.
  void set_on_deinitialized_handler(link_initialized_handler&& handler);

  /// Sets the connected callback handler, which will be called on every successful connect to the broker.
  /// @param handler The handler to call upon connect to the broker.
  void set_on_connected_handler(link_connected_handler handler);

  /// Sets the disconnected callback handler, which will be called on every disconnect from the broker.
  /// @param handler The handler to call upon disconnect from the broker.
  void set_on_disconnected_handler(link_disconnected_handler handler);

  /// Schedules the given task on the link thread pool. Will be called asynchronously. As the tasks function will be
  /// called on the links thread pool, the task should not block or do time consuming computations.
  /// @param task The task function to call.
  void schedule_task(std::function<void()>&& task);

  /// Schedules the given task on the link thread pool to be called after a given delay. Will be called asynchronously.
  /// As the tasks function will be called on the links thread pool, the task should not block or do time consuming
  /// computations.
  /// @param delay The time to delay the tasks function call.
  /// @param task The task function to call.
  template <class Rep, class Period>
  void schedule_timed_task(const std::chrono::duration<Rep, Period>& delay, std::function<void()>&& task)
  {
    do_schedule_timed_task(std::chrono::duration_cast<std::chrono::milliseconds>(delay), std::move(task));
  }

private:
  void do_schedule_timed_task(const std::chrono::milliseconds& delay, std::function<void()>&& task);

  void initialize();
  void deinitialize();

  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
