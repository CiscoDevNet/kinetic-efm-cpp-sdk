// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file responder_link.h

#pragma once

#include <efm_link.h>
#include <efm_logging.h>

#include <iostream>
#include <random>
#include <sstream>

using namespace cisco::efm_sdk;


/// Custom action result implementation for easier reuse.
class MyActionValuesResult : public ActionValuesResult
{
public:
  /// Constructs a success action result. Will produce an action values result with a pre-built row.
  /// @param type Marker type for successful actions.
  MyActionValuesResult(const SuccessType& type)
    : ActionValuesResult(type)
  {
    add_value(true).add_value("");
  }

  /// Constructs a success action result. Will produce an action values result with a pre-built row.
  /// @param type Marker type for successful actions.
  /// @param result Adds this result value to the pre-built row.
  MyActionValuesResult(const SuccessType& type, const Variant& result)
    : ActionValuesResult(type)
  {
    add_value(true).add_value(result);
  }

  /// Constructs an error action result. Will produce an action values result with a pre-built row.
  /// @param type Marker type for unsuccessful actions.
  /// @param error_message Adds this error message to the pre-built row.
  MyActionValuesResult(const ErrorType& type, const std::string& error_message)
    : ActionValuesResult(type)
  {
    add_value(false).add_value(error_message);
  }
};

/// Custom error action result implementation for easier reuse.
class ErrorActionValuesResult : public ActionValuesResult
{
public:
  /// Constructs a success action result. Will produce an action values result with a pre-built row.
  /// @param error_text Error text to set.
  ErrorActionValuesResult(const std::string& error_text)
    : ActionValuesResult(ActionError)
  {
    set_error_text(error_text);
  }
};

/// This class demonstrates the ability to put callbacks in other classes/objects then
/// the main (ResponderLink) class
class CallbackDemoClass
{
public:
  /// Constructor
  /// @param responder The responder to act on.
  CallbackDemoClass(Responder& responder)
    : responder_(responder)
  {
  }

  /// This method is called when the value of String (path = '/str') was changed
  /// @note To register this callback see ResponderLink::initialize
  /// @param node_path path the callback was triggered
  /// @param callback_on the requester event that triggers this callback
  void on_value_changed(const cisco::efm_sdk::NodePath& node_path, cisco::efm_sdk::CallbackOn callback_on)
  {
    if (node_path == NodePath{"/str"} && callback_on == cisco::efm_sdk::CallbackOn::ValueChanged) {
      responder_.set_value(
        num_str_changes_path_,
        Variant{++num_str_changes_},
        std::chrono::system_clock::now(),
        [](const std::error_code& ec) {
          if (ec) {
            LOG_EFM_ERROR(ec, "Callback of set_value error");
          }
        });
    }
  }

  /// Returns the node path of /num_subscribe_path
  /// @return The node path to set changes on.
  const NodePath& get_num_str_changes_path() const noexcept
  {
    return num_str_changes_path_;
  }

private:
  Responder& responder_;
  uint64_t num_str_changes_{0};
  NodePath num_str_changes_path_{"/num_str_changes"};
};


/// @brief The responder link example demonstrates the EFM SDK API for responder implementations. Shows node and action
/// creation, stream handling and task scheduling.
class ResponderLink
{
public:
  /// Constructs the responder link implementation.
  /// Adds a node creation pattern to the responder in order to add default actions and configs to nodes created as
  /// children from the /nodes path.
  /// @param link The link to work with.
  ResponderLink(Link& link)
    : link_(link)
    , responder_(link.responder())
    , callbacks_(responder_)
  {
    responder_.add_node_creation_callback(
      path_pattern_,
      std::bind(
        &ResponderLink::nodes_child_created,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3));
  }

  /// Destructs the responder link implementation.
  ~ResponderLink()
  {
    responder_.remove_node_creation_callback(path_pattern_);
  }

  /// The initialize callback that will be called as soon as the initialization including serialization is complete.
  /// Will create the first level node hierarchy. Only nodes not created by the deserialization will actually be
  /// created.
  /// @param link_name The name of the link.
  /// @param ec The error code will be set to an error if the initialization failed.
  void initialize(const std::string& link_name, const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l1, "Responder link '" << link_name << "' initialized");

      NodeBuilder builder{"/"};

      builder.make_node("nodes").display_name("Nodes").config("$custom", Variant{"Done."}).serializable();
      builder.make_node("rng").display_name("Random").type(ValueType::Number);
      builder.make_node("seq")
        .display_name("Sequence")
        .type(ValueType::Number)
        .value(sequence_)
        .on_subscribe(std::bind(&ResponderLink::on_subscribe_seq, this, std::placeholders::_1));
      builder.make_node(callbacks_.get_num_str_changes_path().get_name())
        .display_name("Number of 'String' changes")
        .type(ValueType::Number)
        .value(0);
      builder.make_node("str")
        .display_name("String")
        .type(ValueType::String)
        .value("Hello, World!")
        .writable(Writable::Write, std::bind(&::ResponderLink::set_string, this, std::placeholders::_1));
      builder.make_node("connection status")
        .display_name("Connection Status")
        .type(ValueType::Bool)
        .enum_values("Disconnected,Connected")
        .value(true);

      builder.make_node("echo", "echo_profile")
        .display_name("Echo")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::echo_called,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_param(ActionParameter{"Input", ValueType::String}.placeholder("Tschaka!"))
                  .add_column({"Success", ValueType::Bool})
                  .add_column({"Message", ValueType::String}));

      builder.make_node("get one row")
        .display_name("Get One Row")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::get_one_row,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_column({"Success", ValueType::Bool})
                  .add_column({"Message", ValueType::String}));

      builder.make_node("get multiple rows")
        .display_name("Get Multiple Rows")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::get_multiple_rows,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_column({"Value", ValueType::String})
                  .set_table());

      builder.make_node("get multiple row updates")
        .display_name("Get Multiple Rows And Updates")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::get_multiple_row_updates,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_column({"Value", ValueType::String})
                  .set_table());

      builder.make_node("get stream")
        .display_name("Get Stream")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::get_stream,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_column({"Number", ValueType::Number})
                  .add_column({"String", ValueType::String})
                  .set_stream());

      add_number_editors(builder);
      add_string_editors(builder);
      add_binary_editors(builder);
      add_column_editors(builder);

      responder_.add_node(
        std::move(builder),
        std::bind(&ResponderLink::nodes_created, this, std::placeholders::_1, std::placeholders::_2));

      // Add callback for un/subscribe on node '/str'
      responder_.register_callback(
        str_path_,
        cisco::efm_sdk::CallbackOn::ValueChanged,
        std::bind(&CallbackDemoClass::on_value_changed, &callbacks_, std::placeholders::_1, std::placeholders::_2));
    } else {
      LOG_EFM_ERROR(ec, "could not initialize responder link");
    }
  }

  /// Will be called the node '/str' is set via an \@set action.
  /// @param value The value that was set.
  void set_string(const Variant& value)
  {
    LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "Set str to " << value);
  }

  /// Will be called if a subscribe or unsubscribe is issued for the /seq node.
  /// @param subscribe True if a subscribe was done or false if an unsubscribe was done.
  void on_subscribe_seq(bool subscribe)
  {
    if (subscribe) {
      LOG_EFM_INFO(responder_error_code::subscribed_seq);
    } else {
      LOG_EFM_INFO(responder_error_code::unsubscribed_seq);
    }
  }

  /// Here we demonstrate how to include editors for an input field of type number
  /// @param builder The builder to add the editors to.
  void add_number_editors(NodeBuilder& builder)
  {
    ActionParameters params;
    params.push_back(
      ActionParameter{"Number", ValueType::Number}.description("Just enter a number").editor(editor::Number{}));
    params.push_back(
      ActionParameter{"Color", ValueType::Number}.description("Choose a color").editor(editor::Color("16776960")));
    params.push_back(
      ActionParameter{"Min", ValueType::Number}.description("Enter a value of at least 333").editor(editor::Min(333)));
    params.push_back(
      ActionParameter{"Max", ValueType::Number}.description("Enter a value of at most 777").editor(editor::Max(777)));
    params.push_back(ActionParameter{"Min and max", ValueType::Number}
                       .description("Enter a value between 333 and 777")
                       .editor(editor::MinMax(333, 777)));

    builder.make_node("number editors")
      .display_name("Number Editors")
      .action(Action(
                PermissionLevel::Read,
                std::bind(
                  &ResponderLink::get_action_number_editor,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4))
                .params(std::move(params))
                .add_column({"Success", ValueType::Bool})
                .add_column({"Entered Number ", ValueType::Number})
                .add_column({"Entered Color ", ValueType::Number})
                .add_column({"Entered Min Number ", ValueType::Number})
                .add_column({"Entered Max Number", ValueType::Number})
                .add_column({"Entered Number of MinMax Number ", ValueType::Number}));
  }

  /// Here we demonstrate how to include editors for an input field of type string. Editors are
  /// * Password, hides the input
  /// * Date range, opens a date range editor
  /// * Text area, Allows multi-line input
  /// @param builder The builder to add the editors to.
  void add_string_editors(NodeBuilder& builder)
  {
    using namespace cisco::efm_sdk::editor;

    ActionParameters params;
    params.push_back(ActionParameter{"Password", ValueType::String}
                       .description("Enter a password")
                       .default_value("secretpassword")
                       .placeholder("Password for user Leibniz")
                       .editor(Password{}));
    params.push_back(
      ActionParameter{"Date range", ValueType::String}.description("Opens a date range editor").editor(DateRange{}));
    params.push_back(
      ActionParameter{"Text area", ValueType::String}.description("Enter a multi line text").editor(TextArea{}));

    builder.make_node("string editors")
      .display_name("String Editors")
      .action(Action(
                PermissionLevel::Read,
                std::bind(
                  &ResponderLink::get_action_string_editor,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4))
                .params(std::move(params))
                .add_column({"Success", ValueType::Bool})
                .add_column({"Entered password ;) ", ValueType::String})
                .add_column({"Entered date range ", ValueType::String})
                .add_column({"Entered text in text area  ", ValueType::String}));
  }

  /// There are more editors. Here we demonstrate binary editors.
  /// @param builder The builder to add the editors to.
  void add_binary_editors(NodeBuilder& builder)
  {
    builder.make_node("binary editors")
      .display_name("Binary Editors")
      .action(Action(
                PermissionLevel::Read,
                std::bind(
                  &ResponderLink::get_action_binary_editor,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4))
                .add_param(ActionParameter{"File input", ValueType::Binary}
                             .description("Choose a file")
                             .editor(editor::FileInput{"application/pdf"}))
                .add_column({"Success", ValueType::Bool})
                .add_column({"Chosen file ", ValueType::String}));
  }

  /// There are editors you can put into a action column. Here we demonstrate this kind of editor.
  /// @param builder The builder to add the editors to.
  void add_column_editors(NodeBuilder& builder)
  {
    using namespace cisco::efm_sdk::editor;

    builder.make_node("column editors")
      .display_name("Column Editors")
      .action(
        Action(
          PermissionLevel::Read,
          std::bind(
            &ResponderLink::get_action_column_editor,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4))
          .add_column({"Success", ValueType::Bool})
          .add_column(ActionColumn{"Get a celebrated poetry ", ValueType::Binary}.editor(Download{"Hamlet.txt"})));
  }

  /// Called every time the link connects to the broker.
  /// Will set values on different paths and start scheduled tasks for the random and sequence value generation.
  /// @param ec The error code will be set to an error if the connect failed.
  void connected(const std::error_code& ec)
  {
    if (!ec) {
      disconnected_ = false;
      LOG_EFM_INFO(responder_error_code::connected);

      responder_.set_value(str_path_, cisco::efm_sdk::Variant{"Hello, World!"}, [](const std::error_code&) {});

      link_.schedule_timed_task(std::chrono::seconds(1), [&]() { this->set_rng(); });
    }
  }

  /// Called every time the link is disconnected from the broker.
  /// Will set a flag to signal the disconnected status to the scheduled tasks.
  /// @param ec The error code will be set to an error if the disconnect failed.
  void disconnected(const std::error_code& ec)
  {
    LOG_EFM_INFO(responder_error_code::disconnected, ec.message());
    disconnected_ = true;
  }

  /// The deinitialize callback that will be called as soon as the link is being stopped.
  /// @param link_name The name of the link.
  /// @param ec The error code will be set to an error if the deinitialization failed.
  void deinitialize(const std::string& link_name, const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l1, "Responder link '" << link_name << "' deinitialized");
    } else {
      LOG_EFM_ERROR(ec, "could not deinitialize responder link");
    }
  }

  /// Callback that will be called upon construction of the first level nodes.
  /// Will add an action to the /nodes path. This action could also be added via a node creation pattern callback. The
  /// action will add a new child node to the /nodes path.
  /// @param paths The paths of the nodes that were actually created. A path that was added to the NodeBuilder is
  /// not part of the paths vector means that the node was already created. Normally, there is no need to check for
  /// the presence of a path. If the error code signals no error, just continue with your work.
  /// @param ec The error code will be set to an error if the node creation failed.
  void nodes_created(const std::vector<NodePath>& paths, const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l1, "created nodes");
      for (const auto& path : paths) {
        LOG_EFM_DEBUG("ResponderLink", DebugLevel::l2, "created path - " << path);
      }

      link_.schedule_timed_task(std::chrono::seconds(1), [&]() { this->set_seq(); });
      link_.schedule_timed_task(std::chrono::seconds(10), [&]() { this->create_node_or_set_value_demo(0); });

      NodeBuilder builder{"/nodes"};

      builder.make_node("add_node")
        .display_name("Add Node")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::add_node_called,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_param({"Node Name", ValueType::String})
                  .add_param(ActionParameter{"Value Type", ValueType::Enum}
                               .enum_values("None,String,Number,Bool")
                               .default_value(false))
                  .add_column({"Success", ValueType::Bool})
                  .add_column({"Message", ValueType::String}));

      builder.make_node("enumerate_nodes", "node")
        .display_name("Enumerate Nodes")
        .action(Action(
                  PermissionLevel::Read,
                  std::bind(
                    &ResponderLink::enumerate_nodes_called,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4))
                  .add_column({"Success", ValueType::Bool})
                  .add_column({"Message", ValueType::String}));

      responder_.add_node(std::move(builder), [](const std::vector<NodePath>&, const std::error_code&) {});
    }
  }

  /// Callback called upon scheduling of the randomize task.
  /// Will set the /rng path to a random value. Will schedule a new randomize task if the link is still connected.
  void set_rng()
  {
    auto val = static_cast<uint32_t>(dist_(mt_));
    responder_.set_value(rng_path_, Variant{val}, std::chrono::system_clock::now(), [](const std::error_code&) {});

    if (!disconnected_) {
      link_.schedule_timed_task(std::chrono::seconds(1), [&]() { this->set_rng(); });
    }
  }

  /// Callback called upon scheduling of the sequence task.
  /// Will set the /seq path to a monotonically increasing sequence value. Will schedule a new sequence task if the
  /// link is still connected.
  void set_seq()
  {
    int64_t seq = sequence_.as_int();
    sequence_ = ++seq;
    responder_.set_value(seq_path_, Variant(sequence_), [](const std::error_code&) {});

    link_.schedule_timed_task(std::chrono::seconds(1), [&]() { this->set_seq(); });
  }

  /// This is a demo member function periodically called. An initially not existing node (/cnosv) will be created with a
  /// specified value. On each subsequent call only a new value will be set on the prior created node
  /// @param i is just a counter
  void create_node_or_set_value_demo(uint64_t i)
  {
    uint64_t num = i * i + 1;
    NodeCreateOrSetValueDescription desc("/cnosv", "node", "Create node or set value", ValueType::Number, num);
    responder_.create_node_or_set_value(desc, [](const cisco::efm_sdk::NodePath&, const std::error_code&) {});
    link_.schedule_timed_task(std::chrono::milliseconds(700), [&, i]() { this->create_node_or_set_value_demo(i + 1); });
  }

  /// Action callback for the '/get one row' action. Will send one row of values back to the peer. The stream
  /// will be closed automatically by the link.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_one_row(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess, Variant{"Hello, World!"}}});
    }
  }

  /// Action callback for the '/get multiple rows' action. Will send a table of row values back to the peer. The
  /// stream has to be closed manually upon sending all table data.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_multiple_rows(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      stream->set_close_handler(std::bind(&ResponderLink::close_stream, this));
      auto action_stream = std::unique_ptr<ActionTableResult>{new ActionTableResult{ActionSuccess}};
      for (int n = 0; n < 5; ++n) {
        action_stream->next_row();
        action_stream->add_value("Hello, World!");
      }
      stream->set_result(std::move(action_stream));
      stream->close();
    }
  }

  /// Action callback for the '/get multiple row updates' action. Will send a table of row values back to the
  /// peer. Will start a scheduled task to send more data to the peer.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_multiple_row_updates(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      stream->set_close_handler(std::bind(&ResponderLink::close_stream, this));
      auto action_table_stream = std::unique_ptr<ActionTableResult>{new ActionTableResult{ActionSuccess}};
      for (int n = 0; n < 5; ++n) {
        action_table_stream->next_row();
        action_table_stream->add_value("Hello, World!");
      }
      stream->set_result(std::move(action_table_stream));

      link_.schedule_timed_task(std::chrono::seconds(1), std::bind(&ResponderLink::update_table, this, stream, false));
    }
  }

  /// Will update the stream table open previously. Will send more rows and finally replace the first 5 rows of data
  /// before closing the table.
  ///
  /// @param stream The stream to table to update is attached to
  /// @param ready Indicates if the table is ready
  void update_table(const MutableActionResultStreamPtr& stream, bool ready)
  {
    auto& action_table_stream = stream->get_result_table();

    if (!ready) {
      for (int n = 0; n < 5; ++n) {
        action_table_stream.next_row();
        action_table_stream.add_value("Hello, World!");
      }
      if (stream->commit()) {
        link_.schedule_timed_task(std::chrono::seconds(1), std::bind(&ResponderLink::update_table, this, stream, true));
      }
    } else {
      action_table_stream.set_replace_modifier(0, 4);
      action_table_stream.set_mode(ActionStreamingMode::Append);
      for (int n = 0; n < 5; ++n) {
        action_table_stream.next_row();
        action_table_stream.add_value("Goodbye!");
      }
      stream->close();
    }
  }

  /// Action callback for the '/get stream' action. Will send five rows of values back to the peer. The stream
  /// will be kept open until closed by the peer. Will schedule a task for sending more values.
  /// @param stream The stream to add a result to. It should be used in following calls to send more values to the
  /// peer.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_stream(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      stream->set_close_handler(std::bind(&ResponderLink::close_stream, this));
      auto action_stream = std::unique_ptr<ActionStreamResult>{new ActionStreamResult{ActionSuccess}};

      uint64_t number{0};
      for (int n = 0; n < 5; ++n) {
        action_stream->next_row();
        action_stream->add_value(Variant{++number});
        action_stream->add_value(Variant{"Number " + std::to_string(number)});
      }

      stream->set_result(std::move(action_stream));
      link_.schedule_timed_task(
        std::chrono::seconds(1), std::bind(&ResponderLink::update_stream, this, stream, number));
    }
  }

  /// Action callback for the number editors. Will receive values from the peer.
  /// @param stream The stream to add a result to. It should be used in following calls to send more values to the
  /// peer.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_action_number_editor(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;

    if (!ec) {
      stream->set_result(UniqueActionResultPtr{
        new ActionValuesResult{ActionValuesResult(ActionSuccess)
                                 .add_value(true)
                                 .add_value((params.get("Number") ? *params.get("Number") : Variant{}))
                                 .add_value((params.get("Color") ? *params.get("Color") : Variant{}))
                                 .add_value((params.get("Min") ? *params.get("Min") : Variant{}))
                                 .add_value((params.get("Max") ? *params.get("Max") : Variant{}))
                                 .add_value((params.get("Min and max") ? *params.get("Min and max") : Variant{}))}});
    }
  }

  /// Action callback for the string editors. Will receive values from the peer.
  /// @param stream The stream to add a result to. It should be used in following calls to send more values to the
  /// peer.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_action_string_editor(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;

    if (!ec) {
      stream->set_result(UniqueActionResultPtr{
        new ActionValuesResult{ActionValuesResult(ActionSuccess)
                                 .add_value(true)
                                 .add_value((params.get("Password") ? *params.get("Password") : Variant{}))
                                 .add_value((params.get("Date range") ? *params.get("Date range") : Variant{}))
                                 .add_value((params.get("Text area") ? *params.get("Text area") : Variant{}))}});
    }
  }

  /// Action callback for the binary editors. Will receive values from the peer.
  /// @param stream The stream to add a result to. It should be used in following calls to send more values to the
  /// peer.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_action_binary_editor(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;

    if (!ec) {
      stream->set_result(UniqueActionResultPtr{new ActionValuesResult{
        ActionValuesResult(ActionSuccess)
          .add_value(true)
          .add_value((params.get("File input") ? Variant{"Got a file"} : Variant{"Did not get a file"}))}});
    }
  }

  /// Action callback for the download column editors. Will send a sample text to the user for download
  /// @param stream The stream to add a result to. It should be used in following calls to send more values to the
  /// peer.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void get_action_column_editor(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      stream->set_result(
        UniqueActionResultPtr{new ActionValuesResult{ActionValuesResult(ActionSuccess).add_value(true).add_value(R"(
Sir, in my heart there was a kind of fighting
That would not let me sleep. Methought I lay
Worse than the mutines in the bilboes. Rashly—
And prais'd be rashness for it—let us know
Our indiscretion sometimes serves us well ...
— Hamlet, Act 5, Scene 2, 4–8
)")}});
    }
  }

  /// Callback called upon scheduling of the update stream task. Will send five rows of values to the peer and
  /// schedule a new update stream task if the stream is still open.
  /// @param stream The stream to send values to the peer.
  /// @param number The number used for the value sending.
  void update_stream(const MutableActionResultStreamPtr& stream, uint64_t number)
  {
    auto& action_result_stream = stream->get_result_stream();
    for (int n = 0; n < 5; ++n) {
      action_result_stream.next_row();
      action_result_stream.add_value(Variant{++number});
      action_result_stream.add_value(Variant{"Number " + std::to_string(number)});
    }
    if (stream->commit()) {
      link_.schedule_timed_task(
        std::chrono::seconds(1), std::bind(&ResponderLink::update_stream, this, stream, number));
    }
  }

  /// Callback called upon closing action streams.
  void close_stream()
  {
    LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "closed stream");
  }

  /// Action callback for the '/echo' action. Will send the input parameter send by the peer back to the peer.
  /// The stream will be closed automatically by the link.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void echo_called(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "echo");
      const auto* input = params.get("Input");
      if (input) {
        stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess, *input}});
      }
    }
    stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionError, "Could not set value"}});
  }

  /// Action callback for the add attribute action of a /nodes child node. Will add a custom attribute to the
  /// parent node.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void add_attribute(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "add attribute");

      const auto* name = params.get("Name");
      const auto* value = params.get("Value");

      if (name && value) {
        NodeUpdater updater{parent_path};
        updater.attribute(name->as_string(), *value);
        responder_.update_node(std::move(updater), [stream](const std::error_code& ec) {
          if (!ec) {
            stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess}});
          } else {
            stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionError, "Could not add attribute"}});
          }
        });
      } else {
        stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionError, "Could not add attribute"}});
      }
    }
  }

  /// Action callback for the remove attribute action of a /nodes child node. Will remove a custom attribute from the
  /// parent node.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void remove_attribute(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "remove attribute");

      const auto* name = params.get("Name");

      if (name) {
        NodeUpdater updater{parent_path};
        updater.remove_attribute(name->as_string());
        responder_.update_node(std::move(updater), [stream](const std::error_code& ec) {
          if (!ec) {
            stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess}});
          } else {
            stream->set_result(
              UniqueActionResultPtr{new MyActionValuesResult{ActionError, "Could not remove attribute"}});
          }
        });
      } else {
        stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionError, "Could not remove attribute"}});
      }
    }
  }

  /// Action callback for the add node action of a /nodes child node. Will add a child node to the parent node. The
  /// node creation pattern callback set in the responders constructor will add default actions and configs to the
  /// node.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void add_node_called(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "add node");

      const auto* name = params.get("Node Name");
      if (name) {
        auto action_path = parent_path / name->as_string();

        if (responder_.exists(action_path)) {
          stream->set_result(UniqueActionResultPtr{new ErrorActionValuesResult{"Node already exists"}});
          return;
        }

        NodeBuilder builder{parent_path};
        builder.make_node(name->as_string()).serializable();
        const auto* value_type = params.get("Value Type");
        if (value_type) {
          const std::string& type = value_type->as_string();
          if (type == "String") {
            builder.type(ValueType::String);
          } else if (type == "Number") {
            builder.type(ValueType::Number);
          } else if (type == "Bool") {
            builder.type(ValueType::Bool);
          }
          builder.writable(Writable::Write);
        }

        const auto& child = name->as_string();
        responder_.add_node(
          std::move(builder), [child, stream](const std::vector<NodePath>&, const std::error_code& ec) {
            if (!ec) {
              stream->set_result(
                UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess, "Created node " + child}});
            } else {
              stream->set_result(UniqueActionResultPtr{new ErrorActionValuesResult{"Could not add node"}});
            }
          });
      } else {
        stream->set_result(UniqueActionResultPtr{new ErrorActionValuesResult{"Could not add node"}});
      }
    }
  }

  /// Action callback for the enumerate nodes action of the /nodes node. Will retrieve the paths of all child nodes of
  /// the /nodes path.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void enumerate_nodes_called(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)parent_path;
    (void)params;
    if (!ec) {
      LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "enumerate nodes");

      responder_.enumerate_children("/nodes", [stream](const std::vector<NodePath>& paths, const std::error_code& ec) {
        if (!ec) {
          std::stringstream ss;
          for (const auto& path : paths) {
            ss << path << "\n";
          }
          stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess, ss.str()}});
        } else {
          stream->set_result(UniqueActionResultPtr{new ErrorActionValuesResult{"Could not add node"}});
        }
      });
    }
  }

  /// Action callback for the remove node action of a /nodes child node. Will remove the parent node and all its
  /// children.
  /// @param stream The stream to add a result to.
  /// @param parent_path The path of the node the action was called for.
  /// @param params The parameters set by the peer.
  /// @param ec The error code will be set to an error if the action failed.
  void remove_node_called(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)
  {
    (void)params;
    LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "remove node");

    if (!ec) {
      responder_.remove_node(parent_path, [stream](const std::error_code& ec) {
        if (!ec) {
          stream->set_result(UniqueActionResultPtr{new MyActionValuesResult{ActionSuccess}});
        } else {
          stream->set_result(UniqueActionResultPtr{new ErrorActionValuesResult{"Could not remove node"}});
        }
      });
    }
  }

  /// Node creation pattern callback for the '/nodes/*' pattern. Will be fired for every node created that matches the
  /// pattern. Will add default actions and configs to the node. Will add nothing to action nodes, as the pattern will
  /// match every node, even actions.
  /// @param path The path of the node the pattern matched for.
  /// @param context The context specifies in which operational phase the node was created.
  /// @param ec The error code will be set to an error if the action failed.
  void nodes_child_created(const NodePath& path, NodeCreationContext context, const std::error_code& ec)
  {
    if (ec) {
      LOG_EFM_ERROR(ec, "node creation of path '" << path << "' failed");
      return;
    }
    switch (context) {
      case NodeCreationContext::Normal:
        LOG_EFM_DEBUG(
          "ResponderLink", DebugLevel::l3, "node '" << path << "' was created during normal link operation");
        break;
      case NodeCreationContext::Deserialization:
        LOG_EFM_DEBUG("ResponderLink", DebugLevel::l3, "node '" << path << "' was created during node deserialization");
        break;
    }

    // these paths are the nodes actions and we don't want to add anything to it
    if (
      path.get_name() == "add_node" || path.get_name() == "remove_node" || path.get_name() == "add_attribute"
      || path.get_name() == "remove_attribute" || path.get_name() == "metric") {
      return;
    }

    NodeUpdater updater{path};
    updater.config("custom config", Variant{"custom"});
    responder_.update_node(std::move(updater), [](const std::error_code&) {});

    NodeBuilder builder{path};

    builder.make_node("metric").display_name("Metric").type(ValueType::Number).value(12345);

    builder.make_node("add_node")
      .display_name("Add Node")
      .action(
        Action(
          PermissionLevel::Read,
          std::bind(
            &ResponderLink::add_node_called,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4))
          .add_param({"Node Name", ValueType::String})
          .add_param(
            ActionParameter{"Value Type", ValueType::Enum}.enum_values("None,String,Number,Bool").default_value(false))
          .add_column({"Success", ValueType::Bool})
          .add_column({"Message", ValueType::String}));

    builder.make_node("remove_node")
      .display_name("Remove Node")
      .action(Action(
        PermissionLevel::Read,
        std::bind(
          &ResponderLink::remove_node_called,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4)));

    builder.make_node("add_attribute")
      .display_name("Add Attribute")
      .action(Action(
                PermissionLevel::Read,
                std::bind(
                  &ResponderLink::add_attribute,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4))
                .add_param({"Name", ValueType::String})
                .add_param({"Value", ValueType::String})
                .add_column({"Success", ValueType::Bool})
                .add_column({"Message", ValueType::String}));

    builder.make_node("remove_attribute")
      .display_name("Remove Attribute")
      .action(Action(
                PermissionLevel::Read,
                std::bind(
                  &ResponderLink::remove_attribute,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3,
                  std::placeholders::_4))
                .add_param({"Name", ValueType::String})
                .add_column({"Success", ValueType::Bool})
                .add_column({"Message", ValueType::String}));

    responder_.add_node(std::move(builder), [](const std::vector<NodePath>&, const std::error_code&) {});
  }

private:
  Link& link_;
  Responder& responder_;
  CallbackDemoClass callbacks_;

  const std::string path_pattern_{"/nodes/*"};

  NodePath rng_path_{"/rng"};
  NodePath seq_path_{"/seq"};
  NodePath str_path_{"/str"};
  Variant sequence_{0};

  bool disconnected_{true};

  std::random_device rd_;
  std::mt19937 mt_{rd_()};
  std::uniform_real_distribution<> dist_{0, 2000000};
};
