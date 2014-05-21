<?php

class UWTPageMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = 'Importing Pages for a given department.';
    $this->dependencies = array('UWTUser');


    $query = Database::getConnection('default', 'for_migration');
    $result = $query->select('node', 'n');
    $result->join('field_data_field_site', 't', 't.entity_id=n.nid');
    $result->join('field_data_body', 'b', 'b.entity_id=n.nid');
    $result->fields('n', array('nid', 'title', 'status', 'uid'));
    $result->fields('t', array('entity_id'));
    $result->fields('b', array('body_value', 'body_format', 'body_summary'));
    $result->condition('field_site_tid', 1368); // <-- Change this tid
    $this->source = new MigrateSourceSQL($result);
    $this->destination = new MigrateDestinationNode('page');
    
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'nid' => array('type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            )
          ),
        MigrateDestinationNode::getKeySchema()
        );
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('body', 'body_value');
    $this->addFieldMapping('body:summary', 'body_summary');
    $this->addFieldMapping('body:format', 'body_format');
    // Author of the node
    $this->addFieldMapping('uid','uid')
      ->sourceMigration('UWTUser')
      ->defaultValue(1);
  }

  public function prepareRow($row) {
    // Always include this fragment at the beginning of every prepareRow()
    // implementation, so parent classes can ignore rows.
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    if ($row->body_format == 'wysiwyg') {
      $row->body_format = 'full_html';
    }
    //dpm($row, '$row');
  }

}
