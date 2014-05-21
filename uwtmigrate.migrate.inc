<?php

function uwtmigrate_migrate_api() {
  $api = array(
      'api' => 2,
      'groups' => array(
        'uwt' => array(
          'title' => t('UWT Migrations'),
          ),
        ),
      'migrations' => array(
        'UWTPage' => array(
          'class_name' => 'UWTPageMigration',
          'group_name' => 'uwt',
          ),
        'UWTUser' => array(
          'class_name' => 'UWTUserMigration',
          'group_name' => 'uwt',
          ),
        ),
      );
  return $api;
}
