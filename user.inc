<?php

class UWTUserMigration extends Migration{
  public function __construct($arguments) {
    parent::__construct($arguments);
    // Get the data by writing a query against the source db

    $query = Database::getConnection('default', 'for_migration');
    $result = $query->select('users', 'u');
    $result->join('workbench_access_user', 'wau', 'u.uid=wau.uid');
    $result->fields('u', array('uid','mail','name','created','access','login','status'));
    $result->fields('wau', array('access_id'));
    $result->condition('wau.access_id', 1368); // <-- change this tid

    $this->source = new MigrateSourceSQL($result);
    $this->destination = new MigrateDestinationUser();
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'uid' => array(
            'type' => 'int',
            'not null' => TRUE,
            'alias' => 'u', // <-- The TABLE alias from the query above...
            )   
          ),  
        MigrateDestinationUser::getKeySchema()
        );

    $this->addSimpleMappings(array(
          'created', 
          'access',
          'login',
          'status'
          )
        );
    $this->addFieldMapping('name', 'name')
      ->dedupe('users', 'name');
    $this->addFieldMapping('mail', 'mail')
      ->dedupe('users', 'mail');
  }

  public function prepareRow($row) {
    // Always include this fragment at the beginning of every prepareRow()
    // implementation, so parent classes can ignore rows.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Don't import anonymous or admin user
    if($row->uid === 0 || $row->uid === 1) {
      return FALSE;
    }

  }

}
