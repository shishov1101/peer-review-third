struct ConnectionDataBaseParams {
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    string_view db_name;
    int db_connection_timeout;
};

struct QueryFilterParams {
    int min_age;
    int max_age;
    string_view name_filter;
};

std::optional<DBHandler> GetDBHandler(const ConnectionDataBaseParams &connection_db_params) {
    DBConnector connector(connection_db_params.db_allow_exceptions, connection_db_params.db_log_level);
    DBHandler db;
    if (connection_db_params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(connection_db_params.db_name, connection_db_params.db_connection_timeout);
    } else {
        db = connector.Connect(connection_db_params.db_name, connection_db_params.db_connection_timeout);
    }
    if (!connection_db_params.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }

    return db;
}

DBQuery GetPersonDBQuery(const QueryFilterParams &query_filter_params, const DBHandler& db_handler) {
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << query_filter_params.min_age << " and "s << query_filter_params.max_age << " "s
              << "and Name like '%"s << db_handler.Quote(query_filter_params.name_filter) << "%'"s;
    DBQuery query(query_str.str());
}

vector<Person> LoadPersons(const ConnectionDataBaseParams &connection_db_params, const QueryFilterParams &query_filter_params) {
    auto db_handler = GetDBHandler(connection_db_params);
    if (!db_handler)
    {
        return {};
    }

    auto query = GetPersonDBQuery(query_filter_params, db_handler);

    vector<Person> persons;
    for (auto [name, age] : db_handler.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
